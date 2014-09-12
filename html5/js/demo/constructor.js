function F(){
    console.log('in function F.');
}
var o = new F();
console.log(o.constructor);
console.log(o.toString());
console.log("type of o: ", typeof(o));
console.log("type of F: ", typeof(F));
