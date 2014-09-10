var EventEmitter = require('events').EventEmitter;

function StreamLibrary(resourceName) { 
    var self = this;
    process.nextTick(function(){
        self.emit('start');
    });
    // read from the file, and for every chunk read, do: 
    process.nextTick(function(){
        self.emit('data', 'chunkRead'); 
    });
}
StreamLibrary.prototype.__proto__ = EventEmitter.prototype;

var stream = new StreamLibrary('fooResource');
stream.on('start', function() {
    console.log('Reading has started');
});

stream.on('data', function(chunk) {
    console.log('Received: ' + chunk);
});
