var Service = useRoot("/System/Service.js");
useModule("utils.js");

function formatHex(str) {
    var str2 = "";
    if (typeof str == "string") {
        for (var i = 0; i < str.length; i++) {
            str2 += str.charCodeAt(i).toString(16);
        }
    } else {
        for (var i = 0; i < str.length; i++) {
            var sym = str[i].toString(16);
            if (sym.length < 2){
                sym = "0" + sym;
            }
            str2 += "$" + sym;
        }
    }
    return str2;
}

function RoboPlatformService(params){
    this.units = 0;
    this.strBuffer = '';
    this.usb = null;
    this.serial = null;
    this.connected = {};

    var self = this;

    self.settings = {
        stepsPerUnit: 200,
        feedRate: 200
    };

    this.connect("UsbService").then((usbService) => {
        this.usb = usbService;
        usbService.on('insert', (deviceInfo)=>{
            this.usbDeviceInsertHandler(deviceInfo);
        });
        usbService.on('remove', (deviceInfo)=>{
            this.usbDeviceRemoveHandler(deviceInfo);
        });
    }).catch((err)=>{
        Frame.log(err);
    });

    this.connect("SerialService").then((service) => {
        this.serial = service;


        this.serial.on('serial-disconnected', (com)=>{
            Frame.log(com + " disconnected");
            if (com) {
                if (this.serial){
                    this.serial.removeAllListeners('serial-data-'+com);
                }
                if (this.connected[com]) {
                    delete this.connected[com];
                    this.emit('disconnected', com);
                }
            }
        });

        this.serial.ListPorts().then((ports)=>{
            if (ports && ports.length){
                ports.forEach((port)=>{
                    this.connectSerial(port.comName);
                })
            }
        });

    }).catch((err)=>{
        Frame.log(err);
    });
/*
    this.on('packet', (port, data) =>{
        console.log(port + " --> " + formatHex(data));
    });
*/
    this.Send = function (com, addr, data) {
        return this.sendPacket(com, addr, data);
    };

    this.List = function () {
        return Object.keys(this.connected);
    };

    /*
        this.Connect = function(port, baudRate){
            this.connect("SerialService").then(function (service) {
                self.serial = service;

                service.OpenPort(port, {baudRate: baudRate}).then(function () {
                    self.port = port;
                    self.baudRate = baudRate;
                    self.handler = self.stateHandler;

                    self.SerialService.on("serial-data-" + port, function(data){



                        for(var i = 0; i < data.length; i++){

                                                    if (String.fromCharCode(data[i]) == '<'){
                                                        self.strBuffer = self.handler(self.strBuffer, data[i]);
                                                        self.handler = self.byteHandler;
                                                        return;
                                                    };

                                                    if (String.fromCharCode(data[i]) == '>'){
                                                        self.handler = self.charHandler;
                                                    };
                            self.strBuffer = self.handler(self.strBuffer, data[i]);

                            if (String.fromCharCode(data[i]) == '\n'){

                                var message = self.strBuffer;
                                console.log(self.firmware + ': '+message);
                                self.strBuffer = '';

                                var response = self.parser(message);
                                console.log(self.currentHandler);
                                self.currentHandler(response);

                            }
                        }
                    });
                }).catch(function (error) {
                    console.log(error)
                })
            }).catch(function (error) {
                console.log(error)
            });

        };
    */

    return Service.apply(this, arguments);
}

Inherit(RoboPlatformService, Service, {
    unfragment: function (data) {
      return Array.prototype.concat.apply([], data);
    },

    waitSizeHandler: function (com, data, store) {
        if (data && data.length){
            var size = data[0];
            store.size = size;
            data = data.splice(1);
            store.data = [];
            store.handler = this.waitDataHandler;
            if (data.length){
                this.waitDataHandler.call(this, com, data, store);
            }
        }
    },

    waitCrcHandler: function (com, data, store) {
        if (data && data.length){
            var crc = data[0];
            data = data.splice(1);
            store.handler = this.waitStartHandler;
            store.data = this.unfragment(store.data);
            if (crc == 3){
                this.emit('packet', com, store.data);
                this.emit(com, store.data);
                if (data.length){
                    this.waitStartHandler.call(this, com, data, store);
                }
                return;
            }
            store.crc = 222;
            for (var i = 0; i <= store.data.length; i++){
                store.crc ^= store.data[i];
            }
            if (store.crc == crc){
                this.emit('packet', com, store.data);
                this.emit(com, store.data);
            } else {
                this.emit('wrong-packet', com, store.data);
                console.log("wrong-packet on " + com + " " + formatHex(store.data));
            }

            if (data.length){
                this.waitStartHandler.call(this, com, data, store);
            }
        }
    },

    waitStartHandler: function (com, data, store) {
        if (data && data.length){
            var ind = data.indexOf(1);
            if (ind >= 0){
                data = data.splice(ind + 1);

                store.handler = this.waitSizeHandler;
                if (data.length){
                    this.waitSizeHandler.call(this, com, data, store);
                }
            }
        }
    },

    waitDataHandler: function (com, data, store) {
        if (data && data.length){
            if (data.length < store.size) {
                store.data.push(data);
                store.size -= data.length;
            } else {
                var tail = data.splice(store.size);
                store.data.push(data);
                store.size = 0;
                store.handler = this.waitCrcHandler;
                if (tail.length){
                    this.waitCrcHandler.call(this, com, tail, store);
                }
            }
        }
    },

    serialDataHandler: function (com, data) {
        if (com && this.connected[com]){
            var obj = this.connected[com];
            if (!obj.handler){
                obj.handler = this.waitStartHandler;
            }
            obj.handler.call(this, com, data, obj);
        }
    },

    parseCOM: function(deviceInfo){
        var regex = /\((COM\d+)\)/;
        if (deviceInfo && deviceInfo.deviceName){
            var results = regex.exec(deviceInfo.deviceName);
            if (results && results.length && results.length > 1){
                return results[1];
            }
        }
        return '';
    },

    usbDeviceInsertHandler: function(deviceInfo){
        var com = this.parseCOM(deviceInfo);
        Frame.log(com + " inserted");
        if (com){
            this.connectSerial(com);
        }
    },

    usbDeviceRemoveHandler: function(deviceInfo){
        var com = this.parseCOM(deviceInfo);
        Frame.log(com + " removed");
        if (com) {
            if (this.serial){
                this.serial.removeAllListeners('serial-data-'+com);
                //this.serial.ClosePort(com).catch((err)=>{});
            }
            if (this.connected[com]) {
                delete this.connected[com];
                this.emit('disconnect', com);
            }
        }
    },

    connectSerial:function(com){
        if (this.serial){
            this.serial.on('serial-data-'+com, (data) => {
                this.serialDataHandler(com, data);
            });
            this.serial.OpenPort(com, {baudRate: 115200}).then((pname)=>{
                this.connected[com] = {};
                this.emit("connected", com)
            }).catch((err)=>{
                Frame.log(err);
            })
        }
    },

    sendPacket: function(com, dstAddr, data){
        if (com && this.connected[com] && data && data.length){
            var crc = 222;
            data.forEach((byte) => {
                crc ^= byte;
            });
            var newPack = [1, data.length + 2, 78, dstAddr, ...data, 3];
            return this.serial.Write(com, newPack);
        }
        return null;
    },

});

module.exports = RoboPlatformService;