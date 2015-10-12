var Person = function(){
}
var person = new Person();
if(person.__proto__ == Person.prototype) {
  console.log("The same");
}
