Promise.resolve(42).then(
function onFulfilled(value) {
  console.log(value);
});
Promise.reject(new Error("BOOM!")).catch(function(error){
    console.error(error);
});
