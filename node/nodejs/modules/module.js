var i=0;
function addi(){
    i++;
}
function geti(){
    console.log("value of i:", i);
}
exports.addi = addi;
exports.geti = geti;
