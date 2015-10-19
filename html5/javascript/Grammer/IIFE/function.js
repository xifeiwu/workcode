(function(exports) {
  var value = 
  (
  function(v){
    console.log(v);
  }
  )("hello");
  console.log(value);
  
  var func =
  (function(n, w) {
    'use strict';
    return typeof module == 'object' ?
    function(c) {
      c(require, exports, module);
    }: function(c, d) {
      c = (typeof c == 'function') ? c: d;
      var m = {
        exports: {}
      };
      c(function(n) {
        return w[n];
      },
      m.exports, m);
      w[n] = m.exports;
    };
  })('gaia-component', this);
  
  console.log(func.toString());
  
  
})(window);
