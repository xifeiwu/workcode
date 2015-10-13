(function(exports) {
  //getter
  var log = ['test'];
  var obj = {
    get latest () {
      if (log.length == 0) return undefined;
      return log[log.length - 1]
    }
  }
  console.log (obj.latest); // Will return "test".
  
  //setter
  var expr = "foo";
  var obj = {
    baz: "bar",
    set [expr](v) { this.baz = v; }
  };
  console.log(obj.baz); // "bar"
  obj.foo = "baz";      // run the setter
  console.log(obj.baz); // "baz"
})(window);
