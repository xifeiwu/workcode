var promise = new Promise(function(resolve){
    resolve(42);
});
promise.then(function(value){
    console.log(value);
}).catch(function(error){
    console.error(error);
});
