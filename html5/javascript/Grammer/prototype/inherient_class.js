(function(exports) {
  /* -- 类式继承 -- */
　//先声明一个超类
　function Person(name){
　　　this.name = name;
　}
　//给这个超类的原型对象上添加方法 getName 
　Person.prototype.getName =function(){
 　　return this.name;
　}
　//实例化这个超类
　var a =new Person('Darren1')
　alert(a.getName());
　//再声明类
　function Programmer(name,sex){
 　　//这个类中要调用超类Person的构造函数，并将参数name传给它
 　　Person.call(this,name);
 　　this.sex = sex;
　}
  //这个子类的原型对象等于超类的实例
　Programmer.prototype =new Person();
　//因为子类的原型对象等于超类的实例，所以prototype.constructor这个方法也等于超类构造函数，你可以自己测试一下，如果没这一步，alert(Programmer.prototype.constructor)，这个是Person超类的引用，所以要从新赋值为自己本身
　Programmer.prototype.constructor = Programmer;
　//子类本身添加了getSex 方法
　Programmer.prototype.getSex =function(){
 　　return this.sex;
　}
　//实例化这个子类
　var _m =new Programmer('Darren2','male');
　//自身的方法
　alert(_m.getSex());
　//继承超类的方法
　alert(_m.getName());

  exports.person = a;
  exports._m = _m;
})(window);
