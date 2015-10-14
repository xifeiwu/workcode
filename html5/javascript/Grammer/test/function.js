(function(exports) {
  var noop = function() {};
  var base = {
    component: true,
    attached: noop,
    detached: noop,
  };
  console.log(base);
  noop = function(arg) {console.log(arg);}
  console.log(base);
  base.attached = function(args) {console.log("The parameter is: " + args);}
  console.log(base);
})(window);
