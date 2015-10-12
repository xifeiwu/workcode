function ClassA(sColor) {
    this.color = sColor;
}
ClassA.prototype.sayColor = function () {
    console.log(this.color);
};

function ClassB(sColor, sName) {
    ClassA.call(this, sColor);
    this.name = sName;
}

ClassB.prototype = new ClassA();

ClassB.prototype.sayName = function () {
    console.log(this.name);
};

var classb = new ClassB("red", "mycolor");
console.log(classb);
console.log(classb.prototype);
