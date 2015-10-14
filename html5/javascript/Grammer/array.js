(function(exports) {
  var els = new Array();
  var els2 = [1,2,3,4,5];

  var func = function(item, index, array){
    console.log(index + ": " + item);
  };
  els2.forEach(func);
  [].forEach.call(els2, func);
})(window);
