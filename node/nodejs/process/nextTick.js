process.on('uncaughtException', function(e) {
　　console.log(e);
});
process.nextTick(function() {
　　console.log('tick');
});
process.nextTick(function() {
　　iAmAMistake();
　　console.log('tock');
});
process.nextTick(function() {
　　console.log('tick tock');
});
console.log('End of 1st loop');
