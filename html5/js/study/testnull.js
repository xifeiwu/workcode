function returnobj(){
    obj = new Object();
    obj = null;
    return obj;
}
if(returnobj()){
   console.log("obj is an object.");
}else{
   console.log("obj is null.");
}
