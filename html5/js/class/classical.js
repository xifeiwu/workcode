function Person(name) {
  this.name = name
}
Person.prototype = {
  greet: function () {
    return "Hello world, my name is " + this.name;
  }
};

var frank = new Person("Frank Dijon");
console.log(frank.greet());
var lucy = new Person("Lucy");
console.log(lucy.greet());
console.log(frank.greet());
