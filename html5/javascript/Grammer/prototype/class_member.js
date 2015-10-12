(function(exports) {
  /* -- 封装 -- */
  var _packaging =function(){
    //私有属性和方法
    var name ='Darren';
    var method1 =function(){
    //...
    } 
    //特权属性和方法
    this.title ='JavaScript Design Patterns' ;
    this.getName =function(){
  　  return name;
 　 }
　}
　//共有静态属性和方法
　_packaging._name ='Darren code';
　_packaging.alertName =function(){
 　alert(_packaging._name);
　}
　//共有属性和方法
　_packaging.prototype = {
    init:function(){
    //...
    }
  }
  window._packaging = _packaging;
})(window);
/*
　私有属性和方法：函数有作用域，在函数内用var 关键字声明的变量在外部无法访问，私有属性和方法本质就是你希望在对象外部无法访问的变量。
　特权属性和方法：创建属性和方法时使用的this关键字，因为这些方法定义在构造器的作用域中，所以它们可以访问到私有属性和方法；只有那些需要直接访问私有成员的方法才应该被设计为特权方法。
　共有属性和方法：直接链在prototype上的属性和方法，不可以访问构造器内的私有成员，可以访问特权成员，子类会继承所有的共有方法。
　共有静态属性和方法：最好的理解方式就是把它想象成一个命名空间，实际上相当于把构造器作为命名空间来使用。
*/
