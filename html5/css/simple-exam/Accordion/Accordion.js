/******************************
*                             
*   Name:   Accordion         
*   Author: Aken li           
*   Date:   2008.07.22        
*   Blog:   www.kxbd.com      
*                             
******************************/
var Accordion = Class.create();
Accordion.prototype = {
	initialize:function(id,option){
		this.id = id;
		this.obj = $(id);
		this.option = Object.extend({event:"click",isOne:true,dur:0.3,type:0}, option || {});
		this.isOne = this.option.isOne;//是否只显示一个
		this.dur = this.option.dur;//动画时间，为零时无动画
		this.event = this.option.event;//鼠标事件
		this.type = this.option.type;//展开方向，0:上下展开，1:左右展开
		this.dimsArr = [];//提供给showAll
		this.init();
	},

	init:function() {
		var obj = $(this.id);
		var isShowNum;
		var prop = this.type ? 'width' : 'height';
		obj.childElements().each(function(o,i){
			var o1 = o.down();
			var o2 = o.down(1);
			var content_dims = o2.getDimensions();
			this.dimsArr.push(content_dims);
			this.type ? o2.setStyle({width:"0px"}).makeClipping() : o2.setStyle({height:"0px"}).makeClipping();
			o1.observe(this.event,function(e){
				var oH = parseInt(o2.getStyle(prop));
				if(oH){
					this.hideSub(o2,this.dur,prop);
				}else{
					if (this.isOne && isShowNum != null){
						this.hideSub(obj.childElements()[isShowNum].down(1),this.dur,prop);
					}
					this.showSub(o2,(this.type?content_dims.width:content_dims.height),this.dur,prop);
					isShowNum = i;
				}
			}.bind(this));
		}.bind(this));
	},

	showSub:function(element,oH,dur,prop) {
		new Effect.Morph(element, {
			style: prop+':'+oH+'px',
			transition:Effect.Transitions.linear,
			duration:dur
		});
	},

	hideSub:function(element,dur,prop) {
		new Effect.Morph(element, {
			style: prop+':0px',
			transition:Effect.Transitions.linear,
			duration:dur
		});
	},

	showAll:function() {
		if(!this.isOne){
			$(this.id).childElements().each(function(o,i){
				this.showSub(o.down(1),(this.type?this.dimsArr[i].width:this.dimsArr[i].height),this.dur,(this.type ? 'width' : 'height'));
			}.bind(this));
		}
	},

	hideAll:function() {
		if(!this.isOne){
			$(this.id).childElements().each(function(o){
				this.hideSub(o.down(1),this.dur,(this.type ? 'width' : 'height'));
			}.bind(this));
		}
	}
};
