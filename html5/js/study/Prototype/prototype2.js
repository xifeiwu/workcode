function Class() {}
Class.prototype.funca=function(){console.log("in function a.")};

var SubClass = function() {};
SubClass.prototype = new Class();
SubClass.prototype.constructor = SubClass;
SubClass.prototype.funcb=function(){console.log("in function b.")};
SubClass.funcc=function(){console.log("in function c.")};

var subObj = new SubClass();
