RpCompiler = {};

RpCompiler.Compile = function (cnc, text, callback) {
    var context = new RpCodeProcessor(cnc);
    var code = context.Compile(text, callback);

    return code;
}

RpCodeProcessor = function (cnc) {
    this.cnc = cnc;
    this.x = lx;
    this.y = ly;
    this.z = lz;
    this.x = parseInt(this.x);
    this.y = parseInt(this.y);
    this.z = parseInt(this.z);
    this.speed = this.cnc.speed;
};

RpCodeProcessor.prototype = {
    Compile: function (text, callback) {
        var code = [];
        var svg = DOM.div(".svg-codes");
        svg.innerHTML = text;
        svg = svg.get("svg");
        if (!svg) return;
        svg = WS.ExtendElement(svg);
        var svgWidth = svg.attr("width");
        var svgHeight = svg.attr("height");
        var viewBox = svg.attr("viewBox");
        var xRatio = parseFloat(svg.attr("x-ratio"));
        var yRatio = parseFloat(svg.attr("y-ratio"));
        this.speed = parseFloat(svg.attr("speed"));
        if (!xRatio && !yRatio){
            if (svgWidth && svgHeight && viewBox){
                viewBox = viewBox.split(" ");
                viewBox = {
                    width : parseFloat(viewBox[2]) - parseFloat(viewBox[0]),
                    height : parseFloat(viewBox[3]) - parseFloat(viewBox[1]),
                }
                svgWidth = svgWidth.replace("mm", "");
                svgHeight = svgHeight.replace("mm", "");
                svgWidth = parseFloat(svgWidth);
                svgHeight = parseFloat(svgHeight);
                if (
                    svgWidth && !isNaN(svgWidth) &&
                    svgHeight && !isNaN(svgHeight) &&
                    viewBox.height && !isNaN(viewBox.height) &&
                    viewBox.width && !isNaN(viewBox.width)
                ){
                    xRatio = svgWidth / viewBox.width;
                    yRatio = svgHeight / viewBox.height;
                }
            }
        }
        if (!xRatio) xRatio = 1;
        if (!yRatio) yRatio = 1;
        this.nx = this.cnc.mmCoefX * xRatio;
        this.ny = this.cnc.mmCoefY * yRatio;
        this.zg = this.cnc.zGValue;
        this.maxX = 0;
        this.maxY = 0;
        if (!this.zg) this.zg = 1;
        var paths = svg.all("path,ellipse,circle,rect");
        var i = 0;
        this.lx = this.x;
        this.ly = this.y;
        this.lz = this.z;
        this.subX = this.lx;
        this.subY = this.ly;
        if (isNaN(this.speed)){
            this.speed = this.defaultSpeed();
        }
        this.xStart =svg.attr("x-start");
        this.yStart = svg.attr("y-start");
        var startDef = true;
        this.xStart = Math.round(parseFloat(this.xStart) * this.nx);
        if (isNaN(this.xStart)){
            startDef = false;
            this.xStart = this.lx;
        }
        this.yStart = Math.round(parseFloat(this.yStart) * this.ny);
        if (isNaN(this.yStart)){
            startDef = false;
            this.yStart = this.ly;
        }

        this.moveVectors = [];
        this.addLine(code, CNC.GCommands.G, this.xStart, this.yStart, this.z);
        this.drillCicle(code);
        this.spindleUp(code);
        /*	var line1 = this.addLine(code, CNC.GCommands.M, 100, 0, 0, this.speed);
         this.drillCicle(code);
         var line2 = this.addLine(code, CNC.GCommands.M, 0, 100, 0, this.speed);
         this.drillCicle(code);
         var line3 = this.addLine(code, CNC.GCommands.M, -100, 0, 0, this.speed);
         this.drillCicle(code);
         var line4 = this.addLine(code, CNC.GCommands.M, -0, 100, 0, this.speed);
         this.drillCicle(code);
         */
        for (var j = 0; j < paths.length; j++) {
            var path = WS.ExtendElement(paths[j]);
            if (path.tagName.toLowerCase() == "path") {
                var res = this.processPath(path, paths[j]);
                if (res){
                    code = code.concat(res);
                }
            }
        }
        /*
         for (var zvalue = this.cnc.zGValue - 50; zvalue <= this.cnc.zGValue; zvalue += 10){
         this.zg = zvalue;
         for (var j = 0; j < paths.length; j++) {
         var path = WS.ExtendElement(paths[j]);
         if (path.tagName.toLowerCase() == "path") {
         var res = this.processPath(path, paths[j]);
         if (res){
         code = code.concat(res);
         }
         }
         }
         this.followStart(code);
         }
         */	/*
         var vector = this.moveVectors.length-1;
         for (var j = this.moveVectors.length-2; j >= 0 ; j--) {
         if (vector && this.moveVectors[j]){
         var point = this.moveVectors[j];
         if (vector.spindle){
         this.spindleDown(code);
         }
         else{
         this.spindleUp(code);
         }
         this.addLine(code, CNC.GCommands.G, point.x, point.y, this.z, vector.speed);
         vector = point;
         }
         }
         */
        this.followStart(code);

        for (var j = 0; j < paths.length; j++) {
            if (path.tagName.toLowerCase() == "circle" || path.tagName.toLowerCase() == "ellipse" ) {
                var res = this.processCircle(path, paths[j]);
                if (res){
                    code = code.concat(res);
                }
            }
        }

        /*line1.x = this.maxX;
         line2.y = this.maxY;
         line3.x = -this.maxX;
         line4.y = -this.maxY;
         */

        this.followStart(code);
        return code;
    },

    processCircle : function(elem){
        var code = [];
        var coords = {
            x : parseFloat(elem.attr("cx")),
            y : parseFloat(elem.attr("cy")),
            rx :  parseFloat(elem.attr("rx")),
            ry :  parseFloat(elem.attr("rx"))
        }
        if (!coords) {
            return null;
        }
        this.spindleUp(code);
        this.command = CNC.GCommands.G;
        var pcoords = [{x : Math.round(coords.x * this.nx), y : Math.round(coords.y * this.ny)}];
        this.MoveCommand(code, pcoords);
        this.drillCicle(code);
        return code;
    },

    drillCicle : function(code){
        this.spindleDown(code, true);
        this.spindleUp(code);
        this.spindleDown(code, true);
        this.spindleDown(code, true);
        this.spindleUp(code);
        this.spindleUp(code);
    },

    processPath : function(elem){
        var code = [];
        var coords = elem.attr("d");
        this.coordsMemory = [];
        if (!coords) {
            return null;
        }
        this.spindleUp(code);
        this.frun = true;
        var regex = "[mlcaMLCA](?:\\s*\\-?\\d+(?:[.]\\d+)?(?:e-\\d)?,\\s*\\-?\\d+(?:[.]\\d+)?(?:e-\\d)?)+|[zZ]";
        this.command = CNC.GCommands.G;
        var matches = coords.match(new RegExp(regex, 'g'));
        for (var k = 0; k < matches.length; k++) {
            var parse = matches[k]; //.match(new RegExp(regex));
            var sc = parse[0];
            if ("MLCZA".indexOf(sc) >= 0){
                this.absolute = true;
                this.command = CNC.GCommands.G;
            }
            if ("mlcza".indexOf(sc) >= 0){
                this.absolute = false;
                this.command = CNC.GCommands.M;
            }
            if (sc == 'Z' || sc == 'z'){
                this.CloseCommand(code);
                continue;
            }
            parse = parse.substr(1);
            parse = parse.split(' ');
            var pcoords = [];
            for (var pcounter = 0; pcounter < parse.length; pcounter++){
                if (parse != ""){
                    if (parse[pcounter].indexOf(",") <= 0){
                        continue;
                    }
                    var coord = parse[pcounter].split(",");
                    if (coord.length != 2){
                        continue;
                    }
                    var x = parseFloat(coord[0]);
                    var y = parseFloat(coord[1]);
                    if (isNaN(x) || isNaN(y)) {
                        continue;
                    }
                    x = Math.round(x * this.nx);
                    y = Math.round(y * this.ny);
                    pcoords.push({x : x, y : y});
                }
            }
            if (sc == 'M' || sc == 'm'){
                this.MoveCommand(code, pcoords);
            }
            if (sc == 'L' || sc == 'l'){
                this.LineCommand(code, pcoords);
            }
            if (sc == 'C' || sc == 'c'){
                this.CurveCommand(code, pcoords);
            }
        }
        if ((elem.attr('fill') != 'none') && (elem.attr('fill'))){FillCommand(code)}
        return code;
    },

    MoveCommand : function(code, coords){
        var coord = coords[0];
        if (this.absolute){
            coord.x += this.xStart;
            coord.y += this.yStart;
            coord.z = this.z;
        }
        else{
            coord.z = 0;
        }
        this.spindleUp(code);
        this.addLine(code, this.command, coord.x, coord.y, coord.z);
        this.subX = this.x;
        this.subY = this.y;
        if (coords.length > 1){
            coords.shift();
            this.LineCommand(code, coords);
        }
    },

    LineCommand : function(code, coords){
        this.spindleDown(code);
        for (var pcounter = 0; pcounter < coords.length; pcounter++){
            var coord = coords[pcounter];
            if (this.absolute){
                coord.x += this.xStart;
                coord.y += this.yStart;
                coord.z = this.z;
            }
            else{
                coord.z = 0;
            }
            /*	if (coord.x > this.x || coord.y > this.y){
             var xBack = this.x;
             var yBack = this.y;
             this.spindleUp(code);
             this.addLine(code, this.command, coord.x, coord.y, coord.z);
             this.spindleDown(code);
             this.addLine(code, this.command, xBack, yBack, coord.z);
             this.spindleUp(code);
             this.addLine(code, this.command, coord.x, coord.y, coord.z);
             }
             else{
             this.addLine(code, this.command, coord.x, coord.y, coord.z);
             }*/
            this.addLine(code, this.command, coord.x, coord.y, coord.z);
        }
    },

    CurveCommand : function(code, coords){
        var csteps = 10;
        var c0 = { x: this.x, y : this.y };
        var c1 = coords[0];
        var c2 = coords[1];
        var c3 = coords[2];
        if (this.absolute){
            c1.x += this.xStart;
            c1.y += this.yStart;
            c2.x += this.xStart;
            c2.y += this.yStart;
            c3.x += this.xStart;
            c3.y += this.yStart;
        }
        else{
            c1.x += c0.x;
            c1.y += c0.y;
            c2.x += c0.x;
            c2.y += c0.y;
            c3.x += c0.x;
            c3.y += c0.y;
        }
        this.spindleDown(code);
        for (var i = 1/csteps; i <= 1; i += 1/csteps){
            var o1 = {x : (c1.x - c0.x) * i + c0.x, y : (c1.y - c0.y) * i + c0.y};
            var o2 = {x : (c2.x - c1.x) * i + c1.x, y : (c2.y - c1.y) * i + c1.y};
            var o3 = {x : (c3.x - c2.x) * i + c2.x, y : (c3.y - c2.y) * i + c2.y};
            var t1 = {x : (o2.x - o1.x) * i + o1.x, y : (o2.y - o1.y) * i + o1.y};
            var t2 = {x : (o3.x - o2.x) * i + o2.x, y : (o3.y - o2.y) * i + o2.y};
            var coord = {x : (t2.x - t1.x) * i + t1.x, y : (t2.y - t1.y) * i + t1.y};
            this.addLine(code, CNC.GCommands.G, Math.round(coord.x), Math.round(coord.y), this.z);
        }
    },

    FillCommand : function(code){
        var arr = this.coordsMemory;
        while (arr){
            for (var y = 0; y += 2; arr.length){
                this.addLine(code, CNC.GCommands.M, arr[y][0], arr[y][1]);
                this.addLine(code, CNC.GCommands.G, arr[y][0], arr[y][2]);
                this.addLine(code, CNC.GCommands.M, arr[y+1][0], arr[y+1][2]);
                this.addLine(code, CNC.GCommands.G, arr[y+1][0], arr[y+1][1]);
                arr[y].splice(1,2);
                arr[y+1].splice(1,2);
                if (arr[y].length == 1){arr.splice(y,1)};
                if (arr[y+1].length == 1){arr.splice(y+1,1)};
            };
        };
    },

    CloseCommand : function(code, coords){
        if (this.isSpindleDown()){
            //this.closePath(code);
        }
        this.spindleUp(code);
    },

    closePath : function(code){
        if (this.absolute){
            return this.addLine(code, CNC.GCommands.G, this.subX, this.subY, this.z);
        }
        else{
            return this.addLine(code, CNC.GCommands.M, this.subX - this.x, this.subY - this.y, this.z);
        }
    },

    followStart : function(code){
        this.spindleUp(code);
        return this.addLine(code, CNC.GCommands.G, this.xStart, this.yStart, this.z);
    },

    spindleUp : function(code){
        if (this.spindleActive){
            this.spindleActive = 0;
            code.push({command:CNC.GCommands.S1, x:0, y:0, z:0, speed:0});
        }
        if (this.z > this.lz){
            code.push({command:CNC.GCommands.M, x:0, y:0, z: -this.zg, speed:this.speed});
            this.z -= this.zg;
        }
        return null;
    },

    isSpindleDown : function(){
        //return this.z > this.lz;
        return this.spindleActive > 0;
    },

    spindleDown : function(code, checkDisable){
        if (!this.spindleActive){
            this.spindleActive = 1;
            code.push({command:CNC.GCommands.S1, x:0, y:0, z:0, speed:255});
        }
        if (this.lz >= this.z){
            code.push({command:CNC.GCommands.M, x:0, y:0, z: +this.zg, speed:this.speed});
            this.z += this.zg;
        }
        return null;
    },

    addLine : function(code, command, x, y, z, speed, upsert){
        var line = code[code.length - 1];
        if (upsert) line = { command: CNC.GCommands.M, x : 0, y: 0, z: 0, speed: this.speed };
        if (this.frun){
            if (command == CNC.GCommands.M){
                x = x - (this.x - this.xStart);
                y = y - (this.y - this.yStart);
            }
            this.frun = false;
        }
        if (line && line.command == command && line.speed == this.speed){
            if (line.z == z && line.x == x && line.y == y){
                if (command == CNC.GCommands.M){
                    line.x += x;
                    line.y += y;
                    line.z += z;
                    this.x += x;
                    this.y += y;
                    this.z += z;
                }
                return line;
            }
            if (command == CNC.GCommands.M){
                if (line.x == x && line.y == y) {
                    line.z += z;
                    this.z += z;
                    return line;
                }
                if (line.x == x && line.z == z) {
                    line.y += y;
                    this.y += y;
                    return line;
                }
                if (line.z == z && line.y == y) {
                    line.x += x;
                    this.x += x;
                    return line;
                }
            }
        }
        if (x == 0 && y == 0 && z == 0 && command == CNC.GCommands.M){
            return line;
        }
        line = { command: command, x: x, y: y, z: z, speed: speed === undefined ? this.defaultSpeed() : speed };
        if (command == CNC.GCommands.G){
            this.x = line.x;
            this.y = line.y;
            this.z = line.z;
        }
        if (command == CNC.GCommands.M){
            this.x += line.x;
            this.y += line.y;
            this.z += line.z;
        }
        if (this.x > this.maxX) this.maxX = this.x;
        if (this.y > this.maxY) this.maxY = this.y;
        //this.speed = line.speed;
        if (upsert) code.unshift(line);
        else code.push(line);
        this.moveVectors.push({x : this.x, y: this.y, z: this.z, speed: line.speed, spindle : this.isSpindleDown() });
        return line;
    },

    defaultSpeed: function (ord) {
        var c =	this.speed;
        if (!c && ord){
            c = this.cnc[ord.toLowerCase() + "Speed"];
        }
        if (!c){
            c = this.cnc.speed;
        }
        return c;
    },

    memoryCoords: function(coordx, coordy){
        var arr = this.coordsMemory;
        steps1 = Math.round(
            (Math.sqrt(
                Math.pow((currentX + x), 2) + Math.pow((currentY + y), 2))
            - Math.sqrt(
                Math.pow((currentX),2) + Math.pow((currentY),2)))
            / stepLength);

        steps2 = Math.round((Math.sqrt(Math.pow((width - currentX - x), 2)
                + Math.pow((currentY + y),2)) - Math.sqrt(Math.pow((width - currentX),2)
                + Math.pow(currentY, 2))) / stepLength);

        if (steps1 < 0){
            var k1 = -1;
            steps1 = Math.abs(steps1);
        }else {
            var k1 = 1;
        };

        if (steps2 < 0){
            var k2 = -1;
            steps2 = Math.abs(steps2);
        }else {
            var k2 = 1;
        };
        var error = 0;

        if (steps1 < steps2){
            var  a = steps1;
            steps1 = steps2;
            steps2 = a;
            l1 = length2;
            l2 = length1;
            var flag = false;
        }else{
            var l1 = length1;
            var l2 = length2;
            var flag = true;
        };

        var deltaerr = steps2;

        for(i = 0; i < steps1; i++) {
            l1 += k1*stepLength;
            error = error + deltaerr;
            if (2 * error >= steps1) {
                l2 += k2*stepLength;
                error = error - steps1;
            };
            var x = (l1*l1 - l2*l2)/(2*width) + width/2;
            var y = Math.sqrt(Math.abs(l1*l1 - x*x));
            var i = 0;
            while (!addY){
                if (arr[i][0] === y){
                    var j = 1;
                    while (!addX){
                        if (arr[i][j] > x){
                            arr[i].splice(j-1,0,x);
                            var addX = true;
                        };
                        j++;
                    };
                    var addY = true;
                };
                if (arr[i][0] > y){
                    arr.splice(i-1,0,[]);
                    arr[i] = [y, x];
                    var addY = true;
                };
                i++;
            };
        };

        if (flag == true){
            length1 = l1;
            length2 = l2;
        }else{
            length1 = l2;
            length2 = l1;
        }

        this.coordsMemory = arr;
    }
}

RegisterCompiler('rp', RpCompiler);