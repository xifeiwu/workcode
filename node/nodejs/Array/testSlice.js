function slice(){
    result = Array.prototype.slice.call(arguments);
    console.log("arguments: ", arguments);
    console.log("result: ", result);
    console.log("json result: ", JSON.stringify(result));
}
//slice(1,2,3,4,5,67,87,99);
slice(true, {"title": "Title"});
