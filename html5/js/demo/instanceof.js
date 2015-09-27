function alert() {
  console.log(arguments[0]);
}

function func() {
  console.log("in function.");
}

function Car(sColor, iDoors, iMpg) {
    this.color = sColor;
    this.doors = iDoors;
    this.mpg = iMpg;
    this.showColor = function () {
        alert(this.color)
    };
}

var oCar1 = new Car("red", 4, 23);


alert(typeof func);
alert(typeof Car);
alert(typeof oCar1);
var s = "Nicholas";
var b = true;
var i = 22;
var u;
var n = null;
var o = new Object();

alert(typeof s);   //string^M
alert(typeof i);   //number^M
alert(typeof b);   //Boolean^M
alert(typeof u);   //undefined^M
alert(typeof n);   //object^M
alert(typeof o);   //object^M


