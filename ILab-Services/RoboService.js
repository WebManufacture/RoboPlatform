var Service = useRoot("/System/Service.js");
var http = useSystem("http");
useModule("utils.js");

function RoboService(params){
    var self = this;
    // это публичная функция:

    this.ports = {coord: '', head: ''};
    this.units = 0;

    this.connectCNC = function(){
        var datahead
        self.on('arduinocoord', function(port){
            console.log(port)
            this.ports.coord = port;
            datahead = this.connection(1);
        })
        self.on('arduinohead', function(port){
            console.log(datahead)
            this.ports.head = port;
        })
        var datacoord = this.connection(0);

        return;
    }

    this.connectCNC();
    this.command = function (command) {
        return new Promise(function (resolve, reject) {
            if (command.units && !self.units){this.units = self.position('G91\n', units)};
            if (!command.units && self.units){this.units = self.position('G90\n', units)};
            switch (command.type) {
                case 'Go':
                    self.move(command, 'G0');
                    break;
                case 'Do':
                    self.move(command, 'G1');
                    break;
                case 'Home':
                    self.home(command);
                    break;
                case 'PWM':
                    self.pwm(command);
                    break;
                case 'state':
                    self.state(command);
                    break;
                default:
                    console.log('unknown command' + command.type);
                    var error = new Error('Unknown command');
                    reject(error);
            }
        })

    };





    return Service.call(this, "RoboService");
}

RoboService.serviceId = "RoboService";

Inherit(RoboService, Service, {
    connection: function(enter) {
        var self = this;

        console.log(enter);
        var count = 0;

        var types = ['coord', 'head'];
        var type = types[enter];
        var port;
        var Arduino={};
        var unoFirstAnswer = "\nGrbl 0.9j ['$' for help]\n";
        var megaFirstAnswer = "\nTransformation matrix: 1.000000 0.000000 0.000000 0.000000 1.000000 0.000000 0.000000 0.000000 1\n";

        this.connect("SerialService").then(function (service) {
            SerialService = service;
            SerialService.ListPorts()
                .then(list => Promise.all(list.map((port, i, arr) => {
                    console.log('74'+JSON.stringify(port));
                    SerialService.OpenPort(port.comName, {baudRate: 115200})
                    .then(portName => new Promise((res, rej) => {
                        SerialService.once("serial-string-" + portName, res);
                        global[type+'portName']  = portName
                    }))
                    .then(name => {
                        console.log(name);
                        portName = global[type+'portName'];

                        if(name.indexOf("Grbl")>-1 && type == 'coord')
                        {
                            console.log('83');

                            console.log('86');
                            console.log('uno was connected on ' + portName);

                            port = portName;
                            self.emit('arduino'+type, [Arduino, port]);
                            count++;
                        }
                        if(name.indexOf("start")>-1 && type == 'head')
                        {

                            console.log('96');
                            console.log('mega was connected on ' + portName);
                            port = portName;
                            self.emit('arduino'+type, [Arduino, port]);
                            count++;
                        }

                        if (count == 0){
                            console.log('closing');

                            Arduino.ClosePort(portName).then(function(){


                            })

                        }

                    })}))
                    .then(() => {









                    })
        )
        }).catch(function (error) {console.log(error)});

    },



    position: function(type, units){
        self.sendCommand(type, type);
        units = !units;
        return units;
    },



    move: function (command, pos) {

        var cUno = pos;
        var cMega = pos;

        if (!command.x && !(command.x == 0)){cUno+=' X' + command.x};
        if (!command.y && !(command.y == 0)){cUno+=' Y' + command.y};
        if (!command.z && !(command.z == 0)){cUno+=' Z' + command.z};
        if (!command.a && !(command.a == 0)){cMega+=' X' + command.a};
        if (!command.b && !(command.b == 0)){cMega+=' Y' + command.b};
        if (!command.c && !(command.c == 0)){cMega+=' Z' + command.c};
        if (!command.speed && !(command.speed == 0)){
            cUno+=' F' + command.speed;
            cMega+=' F' + command.speed;
        };

        self.sendCommand(cUno, cMega);
    },



    home: function (command) {
        var cUno = 'G28';
        var cMega = 'G28';

        if (!command.x && !(command.x == 0)){cUno+=' X' + command.x};
        if (!command.y && !(command.y == 0)){cUno+=' Y' + command.y};
        if (!command.z && !(command.z == 0)){cUno+=' Z' + command.z};

        self.sendCommand(cUno, cMega);
    },



    pwm: function (command) {
        if (!command.e && !(command.e == 0)){
            self.Arduino.head.Send(this.ports.item, 'M104 S'+ command.e, 'ascii').then(function(){
                console.log('command not sent');
            }).catch(function(error){
                console.log(error);
            });
        };
        if (!command.b && !(command.b == 0)){
            self.Arduino.head.Send(this.ports.item, 'M140 S'+ command.b, 'ascii').then(function(){
                console.log('command not sent');
            }).catch(function(error){
                console.log(error);
            });
        };
        if (!command.f && !(command.f == 0)){
            self.Arduino.head.Send(this.ports.item, 'M106 S'+command.f, 'ascii').then(function(){
            console.log('command not sent');
            }).catch(function(error){
                console.log(error);
            });
        };
    },



    state: function (command) {

    },



    sendCommand: function (cUno, cMega) {
        Object.keys(self.Arduino).forEach(function(item, i){
            var command;
            if (item == 'coord'){command = cUno}else{command = cMega}
            self.Arduino.item.Send(this.ports.item, command, 'ascii').then(function(){
                console.log('command not sent');
            }).catch(function(error){
                console.log(error);
            });
        });
    }

});

module.exports = RoboService;


/*
 G0 : Rapid linear Move
 G1 : Linear Move
 Usage
 G0 Xnnn Ynnn Znnn Ennn Fnnn Snnn
 G1 Xnnn Ynnn Znnn Ennn Fnnn Snnn

 G4: Dwell
 Pause the machine for a period of time.
 Parameters
 Pnnn Time to wait, in milliseconds (In Teacup, P0, wait until all previous moves are finished)
 Snnn Time to wait, in seconds (Only on Repetier, Marlin, Smoothieware, and RepRapFirmware 1.16 and later)
 Example
 G4 P200

 G28: Move to Origin (Home)
 Parameters
 This command can be used without any additional parameters.
 X Flag to go back to the X axis origin
 Y Flag to go back to the Y axis origin
 Z Flag to go back to the Z axis origin
 Examples
 G28     ; Home all axes
 G28 X Z ; Home the X and Z axes

 G90: Set to Absolute Positioning

 G91: Set to Relative Positioning

 G92: Set Position
 Parameters
 This command can be used without any additional parameters.
 Xnnn new X axis position
 Ynnn new Y axis position
 Znnn new Z axis position
 Ennn new extruder position

 M0: Stop or Unconditional stop
 Parameters
 This command can be used without any additional parameters.
 Pnnn Time to wait, in milliseconds1
 Snnn Time to wait, in seconds2

 M24: Start/resume SD print

 M25: Pause SD print

 M82: Set extruder to absolute mode

 M83: Set extruder to relative mode

 M104: Set Extruder Temperature
 Parameters
 Snnn Target temperature

 M105: Get Extruder Temperature
 Parameters
 This command can be used without any additional parameters.
 Rnnn Response sequence number1
 Snnn Response type1

 M106: Fan On
 Parameters
 Pnnn Fan number (optional, defaults to 0)2
 Snnn Fan speed (0 to 255; RepRapFirmware also accepts 0.0 to 1.0))
 Extra Parameters
 Innn Invert signal, or disable fan1
 Fnnn Set fan PWM frequency, in Hz1
 Lnnn Set minimum fan speed (0 to 255 or 0.0 to 1.0)1
 Bnnn Blip time - fan will be run at full PWM for this number of seconds when started from standstill1
 Hnn:nn:nn... Select heaters monitored when in thermostatic mode1
 Rnnn Restore fan speed to the value it has when the print was paused1
 Tnnn Set thermostatic mode trigger temperature1

 M107: Fan Off

 M109: Set Extruder Temperature and Wait
 Parameters
 Snnn minimum target temperature, waits until heating
 Rnnn maximum target temperature, waits until cooling (Sprinter)
 Rnnn accurate target temperature, waits until heating and cooling (Marlin)

 M112: Emergency Stop

 M114: Get Current Position

 M119: Get Endstop Status

 M140: Set Bed Temperature (Fast)
 Parameters
 Snnn Target temperature
 Hnnn Heater number1



 */