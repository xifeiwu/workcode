var Person = {
  greet: function () {
    return "Hello world, my name is " + this.name;
  }
};
var frank = Object.create(Person);
frank.name = "Frank Dijon";
console.log(frank.greet());


console.log(Object.create(Person, {name: {value: "Frank Dijon", enumerable: true}}));

