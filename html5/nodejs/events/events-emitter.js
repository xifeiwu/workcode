var events = require('events');
var emitter = new events.EventEmitter();
emitter.on('wantEat', function(thing){
    console.log('小明看到' + thing + '想吃了');
});
emitter.on('wantEat', function(thing){
    console.log('小王看到' + thing + '想吃了');
});
console.log("事件绑定成功，大排挡马上就要出来了。");
setTimeout(function(){
    var thing = "羊肉串";
    console.log("哇", thing, "出来了");
    emitter.emit("wantEat", thing);
}, 1000);
