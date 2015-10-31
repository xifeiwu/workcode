(function(exports) {
	var t = [1,2,3,4,5];

	//some找到数组中第一个符合要求的值后就不在继续执行
	//用来判断数组中是否存符合要求的值，返回结果true|false
	//function返回类型为bool
	var some = t.some(function(item) {
		if(item % 2 === 0) {
		 	return true;
		} else {
			return false;
		}
	});
	console.log("some: " + some);

	//every匹配每一个元素，直到有一个返回false为止
	//function返回类型为布尔
	var every = t.every(function(item) { 
		if(item % 2 !== 0) { 
			return true;
		} else {
		 return false;
		}
	});
	console.log("every: " + every);

	//map处理数组中的所有值并返回处理后的值，不影响原数组，返回结果为新的数组
	//function返回类型为item的类型
	var map = t.map(function(item){
	 return item + 1; 
	});
	console.log("map: " + map);

	//filter数组元素过滤，把返回true的汇集成新的数组，返回结果为新的数组
	//function返回类型为bool
	var filter = t.filter(function(item) {
	 if(item % 2 === 0) {
	  return true;
	 } else {
	  return false;
	 } 
	});
	console.log("filter: " + filter);

})(window);