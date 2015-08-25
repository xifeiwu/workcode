msg="Hello, world"
console.log(msg.toUpperCase());
console.log(msg.split(','));
console.log(true+false);
console.log("7"*"4");
console.log("--");

var scope="global"
function f(){
    console.log(scope);
    var scope="local";
    console.log(scope);
}
//f();

function traverse(){
    o={1:"hello", 2:"world"};
    var keys = Object.keys(o);
    for(var i=0; i<keys.length; i++){
        console.log(keys[i]);
    }
}
//traverse();

function foreach(){
    var data = [1,2,3,4,5];
    data.forEach(function(x){
        console.log(x);
    });
}
//foreach();

function sort(){
    a = ['ant', 'Bug', 'cat', 'Dog'];
    //a.sort();
    //console.log(a); 
    a.sort(function(s,t){
        var a = s.toLowerCase();
        var b = t.toLowerCase();
        if(a<b) return -1;
        if(a>b) return 1;
    });
    console.log(a); 
}
sort();
