(function(exports) {
  /* -- 原型式继承 -- */
  //clone()函数用来创建新的类Person对象
  var clone =function(obj){
   var _f =function(){};
     //这句是原型式继承最核心的地方，函数的原型对象为对象字面量
     _f.prototype = obj; 
     return new _f;
  }
  //先声明一个对象字面量
  var Person = {
     name:'Darren',
     getName:function(){
        return this.name;
     }
  }
  //不需要定义一个Person的子类，只要执行一次克隆即可
  var Programmer = clone(Person);
  //可以直接获得Person提供的默认值，也可以添加或者修改属性和方法
  alert(Programmer.getName())
  Programmer.name ='Darren2'
  alert(Programmer.getName())

  //声明子类,执行一次克隆即可
  var Someone = clone(Programmer);

	window.Person = Person;
	window.Programmer = Programmer;
	window.Someone = Someone;
})(window);
