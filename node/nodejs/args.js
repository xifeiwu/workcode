var test = function(){
    console.log("arguments:", arguments);
    var args = Array.prototype.slice.call(arguments);
    console.log('args', args);
}
test('a', 'b', 'c');
