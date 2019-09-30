var Service = useRoot("/System/Service.js");
useModule("utils.js");
var MotorPacket = require('./cnc.js');

function formatHex(str) {
    var str2 = "";
    if (str && str.length) {
        if (typeof str == "string") {
            for (var i = 0; i < str.length; i++) {
                str2 += str.charCodeAt(i).toString(16);
            }
        } else {
            for (var i = 0; i < str.length; i++) {
                var sym = str[i].toString(16);
                if (sym.length < 2) {
                    sym = "0" + sym;
                }
                str2 += "$" + sym;
            }
        }
    }
    return str2;
}


function RoboMotorService(params){
    this.units = 0;
    this.comPort = '';
    this.currentState = null;
    this.lastCommand = null;
    this.currentHandler = this.initHandler;
    this.rpService = null;

    var self = this;

    self.settings = {
        stepsPerUnit: 200,
        feedRate: 200
    };

        this.connect("RoboPlatformService").then(function (service) {
            self.rpService = service;

            function connectPort(com) {
                self.comPort = com;
                self.getState(self.comPort, 0);

                service.on(com, function (data) {
                   console.log(formatHex(data));
                   self.emit("state", MotorPacket.parse(data));
                });
            }

            service.on("connected", (com) => {
                connectPort(com);
            });

            service.on("disconnected", (com) => {
                service.removeAllListeners(com);
            });

            service.List().then((ports) => {
                if (ports && ports.length) {
                    ports.forEach((port) => {
                        connectPort(port);
                    });
                }
            });

        }).catch(function (error) {
            console.log(error)
        });

    this.sendCommand = function (command) {
        Frame.log('command from ui' + JSON.stringify(command));
        this.doCommand(command);
    };



    this.GetState = function (){
        this.getState(self.comPort, 0);
    };

    this.Move = function (value, speed){
        return new Promise((res, rej) => {
            var mp = new MotorPacket(5, 0);
            mp.value = value ? value : 1000;
            mp.speed = speed ? speed : 1400;
            this.rpService.Send(self.comPort, 0, mp.serialize());
            this.rpService.once(self.comPort, (pack) => {
                res(MotorPacket.parse(pack));
            });
        });
    };



    return Service.call(this, "CNCService");
}

Inherit(RoboMotorService, Service, {

    charHandler: function (str, data) {
        return str + String.fromCharCode(data);
    },

    byteHandler: function (str, data) {
        //console.log('byte:'+data);
        return str + data + ' ';
    },

    position: function(type){
        this.sendCode(type);
        this.units = !this.units;
        return this.units;
    },


    moveParser: function (value, variable) {
        if (!isNaN(parseFloat(value)) && isFinite(value)){

            var nextPosition
            if(interpolation){
                nextPosition = value+this.currentPosition[variable]
            }else{
                nextPosition = value;
            };

            if(nextPosition > this.limit[variable]){return new Error('Limits')};
            return ' ' + variable + value;
        };
    },


    move: function (command) {
        console.log('move('+JSON.stringify(command)+')');
        var self = this;

        var interpolation = '';
        if (command.units){interpolation = 'G91'};

        //var c = '$J=' + interpolation;
        var c = interpolation;
        var nextPosition;
        //  var add = self.moveParser(command.x, 'X');

        if (!isNaN(parseFloat(command.x)) && isFinite(command.x)){
            c+=' X' + command.x;
            if(interpolation){
                nextPosition = command.x + (+self.currentState.x)
            }else{
                nextPosition = command.x;
            };

            if(nextPosition > self.xLimit){
                console.log(JSON.stringify(self.currentState));
                console.log('nextpositionX = ' + nextPosition);
                throw new Error('Limits')
            };

        };


        if (!isNaN(parseFloat(command.y)) && isFinite(command.y)){
            c+=' Y' + command.y
            if(interpolation){
                nextPosition = command.y+(+self.currentState.y);
            }else{
                nextPosition = command.y;
            };

            if(nextPosition > self.yLimit){
                console.log(JSON.stringify(self.currentState));
                console.log('nextpositionY = ' + nextPosition);
                throw new Error('Limits')};
        };
        if (!isNaN(parseFloat(command.z)) && isFinite(command.z)){
            c+=' Z' + command.z
            if(interpolation){
                nextPosition = command.z+(+self.currentState.Z);
            }else{
                nextPosition = command.z;
            };

            if(nextPosition > self.zLimit){
                console.log(JSON.stringify(self.currentState));
                console.log('nextpositionZ = ' + nextPosition);
                throw new Error('Limits')};
        };

        if (!isNaN(parseFloat(command.speed)) && isFinite(command.speed) && command.speed <= 700){
            c+=' F' + command.speed;

        }
        else{
            //c+=' F300'
        };

        if (!isNaN(parseFloat(command.e)) && isFinite(command.e) && command.e <= 700){
            c+=' E' + command.e;

        }

        c+='\n';

        self.currentHandler = self.moveHandler;
        self.sendCode(c);
    },
    /*
    home: function (command) {
        var self = this;

        var c = '$H';
        c+='\n';
        self.sendCode(c);
    },*/


    home: function (command) {
        if (this.firmware == 'Repetier') {
            var c = 'G28';
            command.axises.forEach(function(item){
                c+=' '+item;
            });
            c+='\n';
            this.sendCode(c);
            return;
        }
        this.homeAxises = command.axises;

        this.currentHandler = this.homeHandler;
        this.homeOneAxis();

    },

    homeOneAxis: function(){
        var self = this;
        if (!self.homeAxises[0]) {
            return
        };
        var currentAxis = self.homeAxises[0];

        command = self.axisesSettings[currentAxis].homing(currentAxis);

        self.homing = true;

        self.SerialService.Send(self.port, command, 'ascii').then(function(){
            console.log(command + ' was sent');

        }).catch(function(error){
            console.log('command was not sent');
            console.log(error);
        });
    },

    homeG28Axis: function(axis){
        return 'G28 '+axis+'\n';
    },

    homeCustomAxis: function(axis){
        return '$J= G91 ' + axis + '-200 F' + this.homeFeedRate + '\n';
    },

    homeBackMove: function(){
        var self = this;
        if (!self.homeAxises[0]) {return};

        var currentAxis = self.homeAxises[0];
        command = '$J= G91 ' + currentAxis + self.axisesSettings[currentAxis].homeBackMove + ' F' + self.axisesSettings[currentAxis].feedRate + '\n';

        self.homeAxises.splice(0, 1);

        self.SerialService.Send(self.port, command, 'ascii').then(function(){
            console.log(command + ' was sent');
            self.homeOneAxis();
        }).catch(function(error){
            console.log('command was not sent');
            console.log(error);
        });

    },

    reset: function (){
        var self = this;
        var command = '\x18';
        self.SerialService.Send(self.port, command, 'ascii').then(function(){
            console.log(command + 'command was sent');
        }).catch(function(error){
            console.log('command was not sent');
            console.log(error);
        });
    },


    pwm: function (command) {
        var self = this;
        if (!isNaN(parseFloat(command.e)) && isFinite(command.e)){
            self.sendCode('M104 S'+ command.e);
        };

        if (!isNaN(parseFloat(command.b)) && isFinite(command.b)){
            self.sendCode('M140 S'+ command.b);
        };

        if (!isNaN(parseFloat(command.f)) && isFinite(command.f)){
            self.sendCode('M106 S'+command.f);
        };
    },



    getState: function (com, addr) {
        if (this.rpService) {
            this.rpService.Send(com, addr, [0, 0, 0, 0, 0])
        }
    },

    zProbing: function () {
        var self = this;
        self.currentHandler = self.zProbingHandler;
        for (var i = self.minXbed; i<=self.maxXbed; i+=5){
            self.sendCode('$J= X' + i + '\n');
            self.once('ok', function(message){
                self.sendCode('$J= G91 Z 100 \n');
            });
            self.once('unlocked', function(message){
                self.sendCode('$J= G91 Z -5 \n');
            });
        };
        self.on('programOk', function(){
            self.doCommand(command.program[self.commandNum-n]);
            if (self.commandNum-n == command.program.length){self.currentHandler = self.defaultHandler;}
        });
    },

    zProbingHandler: function (response) {
        var self = this;
        switch (response.type) {
            case 'ok':
                self.emit('ok', response);
                break;
            case 'reset':
                self.reset();
                break;
            case 'lock':
                self.sendCode('$X\n');
                break;
            case 'unlocked':
                console.log('cnc was unlocked');
                //if (self.currentState.x <= self.xLimit)
                break;
            case 'state':
                self.stateParser(response.value);
                break;
        }
    },

    sendCode: function (command, callback) {
        var self = this;
        //command = 'N' + self.commandNum + ' ' + command;
        self.commandNum++;
        self.lastCommand = command;
        self.SerialService.Send(self.port, command, 'ascii').then(function(){
            console.log(command + ' was sent');
        }).catch(function(error){
            console.log('command was not sent');
            console.log(error);
        });

    },

    doCommand: function (command){
        var self = this;
        return new Promise(function (resolve, reject) {
            switch (typeof (command)) {
                case 'string':
                    if (command.indexOf('\n') < 0){command += '\n'};
                    self.sendCode(command);
                    break;
                case 'object':
                    var commands = {
                        Go: self.move,
                        Do: self.move,
                        Home: self.home,
                        PWM: self.pwm,
                        state: self.state,
                        program: self.doProgram
                    };
                    if(!command.type && command.command){
                        command = self.oldToNew(command);
                    };
                    if(command.line){self.currentLine = command.line};
                    if (!commands[command.type]) {

                        console.log('unknown command' + command.type);
                        //var error = new Error('UnknownCommand');
                        //reject(error);
                        return;
                    };
                    commands[command.type].call(self, command);
                    break;
                default:
                    console.log('unknown command' + command.type);
                //var error = new Error('UnknownCommand');
                //reject(error);
            }
        });
    },



    oldCommands: ['unknown', 'Go', 'Rebase', 'Stop', 'state', 'Go', 'Pause', 'Resume', 'spindle1', 'spindle2'],

    oldToNew: function(command){
        command.type = this.oldCommands[command.command];
        command.units = (command.command == 5);

        return command;
    },

    stop: this.reset,

    program: function (command){
        if (typeof command.program == 'string'){
            var arr = [];
            var i = 0;
            var j = 0;
            while (command.program[i]){
                if (command.program[i]!='\n'){
                    arr[j]+=command.program[i];
                }else{
                    j++;
                }
                i++;
            }
            command.program = arr;
        };
        var self = this;
        self.currentHandler = self.programHandler;
        var n = self.commandNum;

        self.doCommand(command.program[0]);

        self.on('programOk', function(){
            self.doCommand(command.program[self.commandNum-n]);
            if (self.commandNum-n == command.program.length){self.currentHandler = self.defaultHandler;}
        });
    },

    parser: function(message){
        if (message.indexOf('rror') > -1){

            return {type: 'error'};
        };
        if (message.indexOf('start') > -1){

            return {type: 'start'};
        };
        if (message.indexOf('Reset') > -1){

            return {type: 'reset'};
        };

        if (message.indexOf('to unlock')>-1){
            return {type: 'lock'};
        };

        if (message.indexOf('Unlocked')>-1){
            return {type: 'unlocked'};
        };


        if (message.indexOf('WPos') > -1){
            return {type: 'state', value: message};
        };

        if(message.indexOf('ok') > -1){
            return {type: 'ok'};
        };

        return {type: 'unknown'};

    },

    initHandler: function(response){
        switch (response.type) {
            case 'start':
                this.firmware = 'Repetier';
                this.currentHandler = this.defaultHandler;
                break;
            case 'lock':
                this.sendCode('$X\n');
                this.firmware = 'grbl';
                break;
            case 'unlocked':
                console.log('cnc was unlocked');
                this.emit('connect', this.port);
                this.sendCode('$$\n');
                this.currentHandler = this.defaultHandler;
                break;
            default:
                console.error('unknown firmware or connection error');
                this.emit('connection-error');

        }


    },

    homeHandler: function(response){
        var self = this;
        switch (response.type) {
            case 'ok':

                break;
            case 'reset':
                self.reset();
                break;
            case 'lock':
                self.SerialService.Send(self.port, '$X\n', 'ascii').then(function(){
                    self.homeBackMove();
                }).catch(function(error){
                    console.log('command was not sent');
                    console.log(error);
                });
                break;
            case 'unlocked':
                console.log('cnc was unlocked');
                break;
            case 'state':
                if(!self.homeAxises[0]){
                    self.currentHandler = self.defaultHandler;
                    self.sendCode('G92 X5 Y5 Z5\n');
                };
                break;
        }
    },

    moveHandler: function(response){

        if(response.type == 'ok'){return};

        if(response.type == 'state'){
            this.stateParser(response.value);
            return;
        };
    },

    programHandler: function(response){
        var self = this;
        if(response.type == 'ok'){
            self.emit('programOk');
            return;
        };
        if(response.type == 'state'){
            self.stateParser(response.value);
            return;
        };
    },

    alarmHandler: function(response){

    },

    errorHandler:function(response){

    },

    defaultHandler:function(response){
        if(response.type == 'state'){
            this.stateParser(response.value);
            return;
        };
    },

    stateParser: function(message){
        var self = this;
        console.log('stateparser')
        var regex = /<([A-Za-z]+),|WPos:([-]?\d+.\d+),([-]?\d+.\d+),([-]?\d+.\d+)/;

        var match = message.match(regex);

        if (match.length){
            self.currentState = {
                state : 2,
                x: match[2],
                y: match[3],
                z: match[4],
                stateA: 0,
                stateB: 0
            };
            if (self.currentLine){self.currentState.line = self.currentLine};
            console.log(self.currentState);
            self.emit('state', self.currentState);
            self.lastState = match[0];
        };
    }

});

module.exports = RoboMotorService;