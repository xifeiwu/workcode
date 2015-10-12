(function(exports){
  var Person = function(){
  }
  var person1 = new Person();
  if(person1.__proto__ == Person.prototype) {
    console.log("The same");
  }
  var person2 = function(){};
  person2.prototype = Person; 
  window.person1 = person1;
  window.person2 = person2;
})(window)
