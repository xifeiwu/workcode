function setName(obj) {
  obj.name = "Nicholas";
  obj = new Object();
  obj.name = "Greg";
}
var person = new Object();
setName(person);
console.log(person.name);


function setProperty(func) {
  func.getName = function() {
    return this.name;
  }
};
var Func = function() {};
setProperty(Func);
