this.x = 9; 
var module = {
  x: 81,
  getX: function() { return this.x; }
};

var value = module.getX(); // 81
console.log(value);

var retrieveX = module.getX;
value = retrieveX(); // 9, because in this case, "this" refers to the global object
console.log(value);

// Create a new function with 'this' bound to module
//New programmers (like myself) might confuse the global var getX with module's property getX
var boundGetX = retrieveX.bind(module);
value = boundGetX(); // 81
console.log(value);
