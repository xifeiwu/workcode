var events = require('events');
var Person = function(name){
    this.name = name;
    this.emitter = new events.EventEmitter(this);
}
Person.prototype.on = function(name, callback){
    this.emitter.on(name, callback);
};
Person.prototype.wantEat = function(thing){
    this.emitter.emit("wantEat", this.name, thing);
};
var xiaoming = new Person("小明");
var xiaowang = new Person("小王");
var emitter = new events.EventEmitter();
xiaoming.on('wantEat', function(name, thing){
    console.log('小明', '想吃', thing);
});
xiaowang.on('wantEat', function(name, thing){
    console.log('小王：',name, '想吃', thing);
});

console.log("事件绑定成功，大排挡马上就要出来了。");
setTimeout(function(){
    var thing = "羊肉串";
    console.log("哇", thing, "出来了");
    xiaoming.wantEat(thing);
    xiaowang.wantEat(thing);
}, 1000);
