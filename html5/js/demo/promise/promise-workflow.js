function asyncFunction() {
//    return new Promise(function (resolve, reject) {
//        setTimeout(function () {
//            resolve('Async Hello world');
//        }, 16);
//    });
    return new Promise(function(resolve, reject) {
        console.log("inner promise.");
        resolve(42);
    });
}

asyncFunction().then(function (value) {
    console.log(value);    // => 'Async Hello world'
}).catch(function (error) {
    console.log(error);
});
console.log("outer promise.");
