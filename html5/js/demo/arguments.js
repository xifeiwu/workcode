function check(args){
    var actual = args.length;
    var expected = args.callee.length;
    console.log("actual: " + actual + "; expected: " + expected);
    if(actual !== expected)
        throw Error("Expected " + expected + "args; got " + actual);
}
function f(x, y, z){
    check(arguments);
    return x+y+z;
}
console.log("f.prototype: " + f.prototype);
console.log("f.toString(): " + f.toString());
console.log(f(1, 2, 3, 4));

