function sayColor(sPrefix,sSuffix) {
    console.log(sPrefix + this.color + sSuffix);
};
cb = function(){
    console.log("Parameters: " + typeof arguments);
    for(var key in arguments){
        console.log(key);
    }
}
var obj = new Object();
obj.color = "blue";

//sayColor.call(obj, "The color is ", "a very nice color indeed.");
cb.call(cb, obj);
