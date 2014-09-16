var events = require('events'),
sys = require('util');

//定义Demo类
var Demo = function(){
    //new一个EventEmitter对象,并将其赋值给this.emitter
    var emitter = new events.EventEmitter();
    this.emitter = emitter;
};

Demo.prototype.p = function(){
    for(var i=0;i < 100; i++){
        if(i == 50){
            this.emitter.emit("data",i);
        }
    }
};

//下面我们就来测试一旦找到50就停止打印.
//new一个Demo对象
var demo = new Demo();

//在demo对象的emitter, emit ‘data’的时候,打印出50
demo.emitter.on('data', function(i){
    console.log(i);
});

//触发
demo.p();
