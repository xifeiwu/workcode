function TestObj() {
  if(arguments.length > 0) {
    this._name = arguments[0];
  }
}
TestObj.prototype = {
  setName: function(name) {
    this._name = name;
  },
  showName: function(){
    console.log(this._name);
  }
}
