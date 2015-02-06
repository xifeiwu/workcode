
function Class() {}

//Use extend to realize inhrietion
//
Class.extend = function extend(props) {
  var prototype = new this();
  var _super = this.prototype;

  for(var name in props) {
    //if a function of subclass has the same name with super
    //override it, not overwrite
    //use this.callSuper to call the super's function
    //
    if(typeof props[name] == "function"
        && typeof _super[name] == "function") {
      prototype[name] = (function(super_fn, fn) {
        return function() {
          var tmp = this.callSuper;
          this.callSuper = super_fn;
          
          var ret = fn.apply(this, arguments);

          this.callSuper = tmp;
          
          if(!this.callSuper) {
            delete this.callSuper;
          }

          return ret;
        }
      })(_super[name], props[name])
    } else {
      prototype[name] = props[name];
    }
  }

  var SubClass = function() {};

  SubClass.prototype = prototype;
  SubClass.prototype.constructor = SubClass;

  SubClass.extend = extend;
  //Use create to replace new
  //we need give our own init function to do some initialization
  //
  SubClass.create = SubClass.prototype.create = function() {
    var instance = new this();

    if(instance.init) {
      instance.init.apply(instance, arguments);
    }

    return instance;
  }

  return SubClass;
}
