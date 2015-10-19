var tmp = new Date();

function f1(){
  console.log(tmp);
  if (false){
    var tmp = "hello world";
  }
}
f1() // undefined

function f2(){
  console.log(tmp);
}
f2() // undefined

//function f3() {
//  let n = 5;
//  if (true) {
//    let n = 10;
//  }
//  console.log(n); // 5
//}
//f3();
