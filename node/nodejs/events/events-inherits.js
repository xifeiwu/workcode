var util = require('util');
var events = require('events');
var Person = function(name){
    this.name = name;
}
util.inherits(Person, events.EventEmitter);

var xiaoming = new Person("小明");
var xiaowang = new Person("小王");
//var emitter = new events.EventEmitter();
xiaoming.on('wantEat', function(thing){
    console.log('小明', '想吃', thing);
});
xiaowang.on('wantEat', function(thing){
    console.log('小王', '想吃', thing);
});

console.log("事件绑定成功，大排挡马上就要出来了。");
setTimeout(function(){
    var thing = "羊肉串";
    console.log("哇", thing, "出来了");
    xiaoming.emit("wantEat", thing);
    xiaowang.emit("wantEat", thing);
}, 1000);
