function hr(){
    console.log("------华丽的分割线------");
}

function sayColor(sPrefix,sSuffix) {
    console.log(sPrefix + this.color + sSuffix);
};

var obj = new Object();
obj.color = "blue";

//sayColor.call(obj, "The color is ", ". a very nice color indeed.");

cb = function(){
    console.log("typeof Parameters: " + typeof arguments);
    console.log("content of Parameters: " + arguments);
    for(var key in arguments){
        console.log(key + ": " + arguments[key]);
    }
}

/**
cb.call(obj, "hello", "world", ["hello", "world"]);
hr();
cb.apply(obj, ["hello", "world", ["hello", "world"]]);
hr();
var newcb = cb.bind(obj, "hello", "world", ["hello", "world"]);
newcb();
*/
function add (a, b) {
  console.log("a: " + a);
  console.log("b: " + b);
  return a + b;
};
//var todo = add.bind(null, 4);
var todo = function () {
  return add.apply(null, [4].concat(Array.prototype.slice.call(arguments)));
};
console.log(todo(7));

//var logHello = console.log.bind(null, "Hello");
//logHello("Mark");
