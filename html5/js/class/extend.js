
function Class() {}
//Class.prototype.init = function(){
//  console.log("in function init of Class.");
//}
//Use extend to realize inhrietion
//
Class.extend = function extend(props) {
  var prototype = new this();
  //console.log("this: " + this);
  //console.log("prototype:");
  for(var key in this.prototype){
    console.log(key);
  }
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


function showClassMethod(mClass){
  for(var key in mClass){
    console.log(key);
  }
}
var mClass = new Class();
mClass.add = function(){};
//showClassMethod(mClass);
//this.callSuper();
var extendClass = Class.extend({
  inita:function(){
    console.log("in function inita of extendClass.");
  },
  initb:function(){
    console.log("in function initb of extendClass.");
  }
})
var extendObj = new extendClass();
extendObj.inita();
extendObj.initb();
console.log("showClassMethod for extendObj");
showClassMethod(extendObj);
console.log("showClassMethod for extendClass");
showClassMethod(extendClass);


//The base Class for Observer classes
//
var Observer = Class.extend({
  init: function(id_) {
    this._id = id_;
  },

  getID: function() {return this._id;},

  registObservers: function() {}
});
console.log(Observer);
