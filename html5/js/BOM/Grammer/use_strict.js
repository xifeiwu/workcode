"use strict";
(function(exports){
  try {
    // 假如有一个全局变量叫做mistypedVariable
    var mistypedVaraible = 17; // 因为变量名拼写错误
    // 这一行代码就会抛出 ReferenceError
  } catch (e) {
    console.log(e);
  }
  
  var run = function(fromWhom){
      arguments[0] = 'alien';
      console.log(fromWhom);
  }
  run('zombie');

  var person = {};
  Object.defineProperty(person, "name", {
    writable: false,
    value: "Nicholas"
  });
  exports.person = person;
})(window);
