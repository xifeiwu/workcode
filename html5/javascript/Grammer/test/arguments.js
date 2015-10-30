(function(exports) {
  function needArgs(){
    console.log(typeof arguments);
    var args = Array.prototype.slice.call(arguments, 0);
    console.log(typeof args);
  }
  needArgs("a", "b", "c");
})(window);
