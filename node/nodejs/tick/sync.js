var func0 = function(req, res, callback){
    console.log('in func0');
    callback(req, res, 0);
}
var func00 = function(req, res, callback){
    console.log('in func00');
    callback(req, res, 10);
}

var func1 = function(req,res,callback){
  setTimeout(function(){
    console.log('in func1');
    callback(req,res,1);  
  },130);
}
var func2 = function(req,res,callback){ 
  setTimeout(function(){
    console.log('in func2');
    callback(req,res,2);
  },50);
}

var func3 = function(req,res,callback){
  setTimeout(function(){
    console.log('in func3');
    callback(req,res,3);   
  },10);
}

var req = 'request';
var res = 'result';
var callback = function(){
    console.log(arguments);
};

func1(req,res,callback);
func2(req,res,callback);
func3(req,res,callback);
func0(req,res,callback);
func00(req,res,callback);

