//****************************************************
//                TaskMenu 3.0
//   		   ¾²¾²µÄÀèÃ÷ 
//           http://www.docode.cn 
//**************************************************** 
/////////////////////////////////////////////////////////////////////////////////
var INF_IS_IEXPLORER = (navigator.userAgent.indexOf("MSIE") == -1)? false : true;
var INF_IS_MENU_BEHAVIOR_AUTO       = true;
var INF_IS_SCROLLBAR_ENABLED        = false;
///////////////////////////////////////////////////////
/////////////*** CONFIG ***////////////////////////////
var CFG_FRAME_COUNT                 = 6;
var CFG_DOCUMENT_MARGIN             = 12;
var CFG_MENU_SPACING                = 15;
var CFG_MENU_WIDTH                  = 185;
var CFG_SCROLLBAR_WIDTH             = 17;
var CFG_TITLEBAR_HEIGHT             = 25;
var CFG_TITLEBAR_LEFT_WIDTH         = 13;
var CFG_TITLEBAR_RIGHT_WIDTH        = 25;
var CFG_TITLEBAR_MIDDLE_WIDTH = CFG_MENU_WIDTH - CFG_TITLEBAR_LEFT_WIDTH - CFG_TITLEBAR_RIGHT_WIDTH;
var CFG_TITLEBAR_BORDER_WIDTH       = 0;
var CFG_MENUBODY_PADDING            = 9;
var CFG_MENUBODY_BORDER_WIDTH       = 1;
var CFG_SLEEP_TIME_BEGIN            = 20;
var CFG_SLEEP_TIME_END              = 60;
var CFG_ALPHA_STEP = Math.ceil( 100 / (CFG_FRAME_COUNT) );
var CFG_IS_SCROLLBAR_ENABLED        = true;
var CFG_IS_SPECIAL_HEAD_NODE        = false;

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
if(new Array().push == null)
{
	//usage: array.push(Array); for ie5.0
	Array.prototype.push = function()
	{
		this[this.length] = arguments[0];
		return this.length;
	}
}
if(new Array().splice == null)
{
	/// usage: array.splice(startPos,deleteCount,[item1,item2,...]);for ie5.0
	Array.prototype.splice = function()
	{
		if(arguments.length < 2 || arguments[1] < 0) 
			return new Array();

		var endPoint1 = arguments[0];	
		if(endPoint1 < 0 && Math.abs(endPoint1) > this.length)
			endPoint1 = 0;

		var startPoint2 = (endPoint1 < 0)? (this.length + endPoint1 + arguments[1]) : (endPoint1 + arguments[1]);

		var bArray = this.slice(0,endPoint1);	
		var dArray = this.slice(endPoint1,startPoint2);
		var eArray = this.slice(startPoint2);
		var nArray = new Array();
		for(var i = 2; i < arguments.length; i++)
		{
			nArray.push(arguments[i]);
		}	
		var fArray = bArray.concat(nArray,eArray);

		for(var i = 0; i < fArray.length; i++)
		{
			this[i] = fArray[i];
		}
		this.length = fArray.length;
		
		return dArray;
	}
}
			
/////////////////////////////////////////////////////////////////		
function TaskMenuState(){}
TaskMenuState.uninited      =  0;
TaskMenuState.open          =  1;
TaskMenuState.close         =  2;
TaskMenuState.opening       =  4;
TaskMenuState.closing       =  8;
TaskMenuState.transforming  = 16;

function TaskMenuItem(strLabel,strIcon,strCommand)
{
	this.label      = strLabel;
	this.icon       = strIcon;
	this.command    = strCommand;
	this.hItem      = null;
	this.hLabel     = null;
	this.hIcon      = null;
	this.hLabelCell = null;
	this.container  = null;
	this.operTimer  = null;
	this.tranTimer  = null;
	this.operStep   = 0;
	this.tranStep   = 0;
	this.operCount  = 0;
	this.tranCount  = 0;

	this.init = function()
	{
		(this.hLabel = document.createElement("FONT")).reflectClass = this;
		with(this.hLabel)
		{	
			reflectClass = this;
		}
		with(this.hItem = document.createElement("TABLE"))
		{	
			cellPadding = 0;
			cellSpacing = 0;
			style.width = "100%";
		}
		(itemRow = this.hItem.insertRow(0)).className="MenuItemRow";
		with(blankCell = itemRow.insertCell(0))
		{
			style.width = (CFG_MENUBODY_PADDING - 2) +"px";
		}
		with(iconCell = itemRow.insertCell(1))
		{
			align  = "RIGHT";
			style.cssText = "width:18px;vertical-align:top;padding-top:3px";						
			with(this.hIcon = iconCell.appendChild(document.createElement("DIV")))
			{
				className = "IconContainer";
			}
		}
		
		with(this.hLabelCell = itemRow.insertCell(2))
		{
			style.padding = "2px " + CFG_MENUBODY_PADDING +"px 2px 5px";
			appendChild(this.hLabel);				
		}
		with(this)
		{
			setIcon(icon);	setLabel(label); setCommand(command);
		}
	}

	this.getLabel = function()
	{
		return this.label;
	}

	this.setLabel = function(strLabel)
	{
		this.hLabel.innerHTML = this.label = strLabel;
	}
	this.getIcon = function()
	{
		return this.icon;
	}

	this.setIcon = function(strIcon)
	{
		this.icon = strIcon;

		while(this.hIcon.firstChild != null)
		{
			this.hIcon.removeChild(this.hIcon.firstChild);
		}

		if(this.icon != null)
		{
			with(this.hIcon)
			{				
				style.width = "18px";
				parentNode.style.width = "18px";
				with(appendChild(new Image()))
				{
					src= this.icon;
				}
			}
		}
		else
		{
			this.hIcon.style.width = "0px";
			this.hIcon.parentNode.style.width = "0px";
		}
	}

	this.getCommand = function()
	{
		return this.command;
	}

	this.setCommand = function(strCommand)
	{
		if( (this.command = strCommand) != null)
		{
			JS.addListener(this.hLabel,"mouseover",TaskMenuItem.onMouseBehavior,false);
			JS.addListener(this.hLabel,"mouseout",TaskMenuItem.onMouseBehavior,false);
			JS.addListener(this.hLabel,"click",TaskMenuItem.onClick,false);
			this.hLabel.className = "MenuItemLabel";
		}		
		else
		{
			JS.removeListener(this.hLabel,"mouseover",TaskMenuItem.onMouseBehavior,false);
			JS.removeListener(this.hLabel,"mouseout",TaskMenuItem.onMouseBehavior,false);
			JS.removeListener(this.hLabel,"click",TaskMenuItem.onClick,false);
			this.hLabel.className = "MenuItemLabelDisabled";
		}
	}

	this.runCommand = function()	
	{
		try
		{	
			eval(this.command);	
		}
		catch(ex)
		{	
			alert("Javascript Error:" + ex.description);	
		}
	}

	this.cloneNode = function()
	{
		return new TaskMenuItem(this.label,this.icon,this.command);
	}

	this.init();
}

TaskMenuItem.onMouseBehavior = function()
{
	with(JS.getSourceElement(arguments[0]))
	{
		if(arguments[0].type.toUpperCase() == "MOUSEOVER")
			className = "MenuItemLabelMouseOver";
		
		else
			className = "MenuItemLabel"; 
	}	
}

TaskMenuItem.onClick = function()
{
	var refClass = JS.getSourceElement(arguments[0]).reflectClass;
	if(refClass != null) refClass.runCommand();
}

function TaskMenu(strLabel,menuState)
{
	this.menuID;
	this.label       = strLabel;
	this.state       = menuState;
	this.items       = [];
	this.hLabel      = null;
	this.hArrayCell  = null;
	this.titlebar    = null;
	this.menuBody    = null;
	this.previous    = null;
	this.next        = null;
	this.background  = null;
	this.operationTimer;
	this.transformTimer;
	this.invalidateTimer
	this.operationStep;
	this.transformStep;
	this.operationSleep;
	this.transformSleep;

	this._getAlphaOpacity = function()
	{
		try
		{
			if(INF_IS_IEXPLORER)	
				return this.menuBody.filters.alpha.opacity;

			else
				return this.menuBody.style.MozOpacity * 100;
		}
		catch(ex)
		{
			return 100;
		}	
	}

	this._setAlphaOpacity = function(n)
	{
		try
		{
			if(INF_IS_IEXPLORER)
				this.menuBody.filters.alpha.opacity = n;

			else
				this.menuBody.style.MozOpacity = n / 100;
		}
		catch(ex)
		{}	
	}
	this._clearItem = function()
	{
		for(var i =0; i< this.items.length; i++)
		{
			this.items[i].container = null;
		}
		this.items = new Array();
	}

	this._clearMenu = function()
	{
		if(this.menuBody == null) return;

		while(this.menuBody.firstChild != null)
			this.menuBody.removeChild(this.menuBody.firstChild);
	}

	this._setMenuFixed = function(bFlag)
	{
		if(this.menuBody == null) return;

		if(bFlag == true)	
		{
			this.menuBody.style.overflow = "hidden";
		}
		else
		{
			this.menuBody.style.overflow = "visible";
			this.menuBody.style.height   = "auto";	
		}			
	}

	this._setMenuHeight = function(height)
	{
		if(INF_IS_IEXPLORER)
			this.menuBody.style.height = height + "px";

		else
			this.menuBody.style.height = ( height - (2 * CFG_MENUBODY_PADDING + CFG_MENUBODY_BORDER_WIDTH) ) + "px";
	}

	this._getMenuHeight = function()
	{
		if(INF_IS_IEXPLORER)
			return this.menuBody.scrollHeight + CFG_MENUBODY_BORDER_WIDTH;

		else
		{
			var scrollHeight = 0;
			for(var i = 0; i < this.menuBody.childNodes.length; i++)
			{
				scrollHeight += this.menuBody.childNodes[i].offsetHeight;			
			}

			return scrollHeight + CFG_MENUBODY_BORDER_WIDTH + 2 * CFG_MENUBODY_PADDING;
		}
	}
	
	this._getScrollWidth = function()
	{
		return (INF_IS_SCROLLBAR_ENABLED)? CFG_SCROLLBAR_WIDTH : 0
	}
	
	this._fixMenuWidth = function(value)
	{
		var titlebarWidth = ( CFG_MENU_WIDTH + ( (INF_IS_IEXPLORER)? 0 : ( -2 * CFG_TITLEBAR_BORDER_WIDTH ) ) );
		var menuBodyWidth = ( CFG_MENU_WIDTH + ( (INF_IS_IEXPLORER)? 0 : ( -2 * CFG_MENUBODY_BORDER_WIDTH ) ) );
		var labelCell = this.titlebar.firstChild.rows[0].cells[1];
		var scrollbarWidth = this._getScrollWidth();

		this.hLabel.style.width   = (CFG_TITLEBAR_MIDDLE_WIDTH - scrollbarWidth) + "px";
		labelCell.style.width     = (CFG_TITLEBAR_MIDDLE_WIDTH - scrollbarWidth) + "px";
		this.titlebar.style.width = (titlebarWidth - scrollbarWidth) + "px";
		this.menuBody.style.width = (menuBodyWidth - scrollbarWidth) + "px";
		
		this._invalidate(true);
	}

	this._fixedMenuPosWhileClose = function()
	{	
		var scrollHeight = this._getMenuHeight();
		this._setMenuHeight(scrollHeight);
		this.menuBody.style.top = (this.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT - scrollHeight) + "px";
		this.menuBody.style.clip = "rect(" + scrollHeight +" auto " + scrollHeight + " auto)";	
		this._setAlphaOpacity(0);
	}

	this._attachState = function(state)
	{
		switch(state)
		{
			case TaskMenuState.open:
			case TaskMenuState.close:
			case TaskMenuState.opening:
			case TaskMenuState.closing:
				this.state >>= 4;
				this.state <<= 4;
			case TaskMenuState.transforming:
				this.state |= state;
			break;
			case TaskMenuState.uninited:
				this.state = state;
			break;
		}
	}

	this._removeState = function(state)
	{
		if(this.state & state)	this.state ^= state;	
	}

	this._invalidate = function(bForce)
	{
		if(INF_IS_MENU_BEHAVIOR_AUTO == true || bForce == true)
		{
			this.invalidate();
		}
	}

	this._extend = function()
	{
		if(this.state & TaskMenuState.close)
		{
			this._fixedMenuPosWhileClose();
			this._removeState(TaskMenuState.transforming);
		}
		else
		{
			if(this.menuBody.offsetHeight + this.transformStep < this._getMenuHeight())
			{
				this._setMenuHeight(this.menuBody.offsetHeight + this.transformStep);
				this.transformTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._extend()",this.transformSleep += 5);
			}
			else
			{
				this._setMenuHeight(this._getMenuHeight());		
				this._removeState(TaskMenuState.transforming);	
			}
		}
		if(this.next != null) this.next._moveTo();
		TaskMenu._refreshAll();
	}

	this._shorten = function()
	{
		if(this.state & TaskMenuState.close)
		{
			this._fixedMenuPosWhileClose();
			this._removeState(TaskMenuState.transforming);
		}
		else
		{	
			if(this.menuBody.offsetHeight + this.transformStep > this._getMenuHeight())
			{	
				this._setMenuHeight(this.menuBody.offsetHeight + this.transformStep);
				this.transformTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._shorten()",this.transformSleep += 5);
			}
			else
			{
				this._setMenuHeight(this._getMenuHeight());
				this._removeState(TaskMenuState.transforming);
			}
		}
		if(this.next != null) this.next._moveTo();
		TaskMenu._refreshAll();
	}

	this._appendItems = function()
	{
		if(this.menuBody == null || this.items.length == 0)return;

		for(var i = 0; i < this.items.length; i++)
			this.menuBody.appendChild(this.items[i].hItem);
	}

	this._isSpecialHeadNode = function()
	{
		return CFG_IS_SPECIAL_HEAD_NODE && this == TaskMenu.headNode;
	}
	
	this._open = function()
	{
		var titlebarBottom = this.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT;

		if( (this.menuBody.offsetTop + this.operationStep) < titlebarBottom )
		{
			this.menuBody.style.top  = (this.menuBody.offsetTop + this.operationStep) + "px";
			this.menuBody.style.clip = "rect(" + (titlebarBottom - this.menuBody.offsetTop) + " auto " + (this.menuBody.offsetHeight - 1) + " auto)";
			this._setAlphaOpacity( this._getAlphaOpacity() + CFG_ALPHA_STEP );
			this.operationTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._open()",this.operationSleep);
			this.operationSleep += 5;
		}
		else
		{
			this.menuBody.style.top  = titlebarBottom + "px";
			this.menuBody.style.clip = "rect(auto auto auto auto)";
			this._attachState(TaskMenuState.open);
			this._setAlphaOpacity(100);
		}
		
		if(this.next) this.next._moveTo();
		TaskMenu._refreshAll();
	}

	this._close = function()
	{
		var menuBodyHeight = this._getMenuHeight();
		var menuBodyBottom = this.menuBody.offsetTop + menuBodyHeight;
		var titlebarBottom = this.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT;

		if( (menuBodyBottom - this.operationStep) > titlebarBottom )
		{
			this.menuBody.style.top = (this.menuBody.offsetTop - this.operationStep) + "px";
			this._setAlphaOpacity( this._getAlphaOpacity() - CFG_ALPHA_STEP );	
			this.operationTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._close()",this.operationSleep);
			this.operationSleep += 5;
		}
		else
		{
			this.menuBody.style.top = ( (titlebarBottom - menuBodyHeight) ) + "px";
			this._setAlphaOpacity(0);
			this._attachState(TaskMenuState.close);
		}
		this.menuBody.style.clip = "rect(" + (Math.abs(titlebarBottom - this.menuBody.offsetTop)) +" auto " + (this.menuBody.offsetHeight - 1) + " auto)";	

		if(this.next) this.next._moveTo();
		TaskMenu._refreshAll();
	}

	this._moveBy = function(dist)
	{}

	this._moveTo = function()
	{	
		var primTop   = this.titlebar.offsetTop;
		var safePoint = this.previous.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT + CFG_MENU_SPACING;
		var dstiTop   = this.previous.menuBody.offsetTop + this.previous.menuBody.offsetHeight + CFG_MENU_SPACING;
		if(dstiTop >= safePoint)
			this.titlebar.style.top = dstiTop + "px";

		else
			this.titlebar.style.top = safePoint + "px";

		if(this.state & TaskMenuState.open)
			this.menuBody.style.top = ( this.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT ) + "px";

		else if(this.state & TaskMenuState.close)
			this.menuBody.style.top = ( this.titlebar.offsetTop + CFG_TITLEBAR_HEIGHT - this.menuBody.offsetHeight) + "px";

		else if(this.state & TaskMenuState.opening || this.state & TaskMenuState.closing)
			this.menuBody.style.top = (this.menuBody.offsetTop + (dstiTop - primTop) ) + "px";

		if(this.next != null) this.next._moveTo();
	}
	
	this.init = function(initFlag)
	{	
		if(initFlag != true && (this.titlebr != null || this.menuBody != null))return;

		this.state = (this.state == false)? TaskMenuState.close : TaskMenuState.open;
		this.label = (this.label == null )? new String() : this.label.toString();
		var fixString = (CFG_IS_SPECIAL_HEAD_NODE == true && TaskMenu.headNode == null)? "_Head" : "";
		with(this.titlebar = document.createElement("DIV"))
		{
			className    = "MenuTitlebar";
			style.top    = ((TaskMenu.headNode == null)? CFG_DOCUMENT_MARGIN : (TaskMenu.rearNode.menuBody.offsetTop + TaskMenu.rearNode.menuBody.offsetHeight + CFG_MENU_SPACING)) + "px";
			style.left   = CFG_DOCUMENT_MARGIN + "px";
			style.width  = (CFG_MENU_WIDTH + ( (INF_IS_IEXPLORER)? 0 : ( -2 * CFG_TITLEBAR_BORDER_WIDTH ) ) - this._getScrollWidth()) + "px";
			style.height = CFG_TITLEBAR_HEIGHT + "px";
		}
		with(titlebarContainer = document.createElement("TABLE"))
		{
			cellPadding = "0";
			cellSpacing = "0";
		}
		with(titlebarRow = titlebarContainer.insertRow(0))
		{
			style.width  = CFG_MENU_WIDTH      + "px";
			style.height = CFG_TITLEBAR_HEIGHT + "px";
		}
		JS.addListener(titlebarRow,"click",TaskMenu._onClick,false);
		JS.addListener(titlebarRow,"mouseover",TaskMenu._onMouseBehavior,false);	
		JS.addListener(titlebarRow,"mouseout",TaskMenu._onMouseBehavior,false);			
		with(leftCell = titlebarRow.insertCell(0))
		{
			className    = "MenuTitlebarLeft"      + fixString;
			style.width  = CFG_TITLEBAR_LEFT_WIDTH + "px";
		}
		leftCell.reflectClass = this;
		with(middleCell = titlebarRow.insertCell(1))
		{
			className    = "MenuTitlebarMiddle"      + fixString;
			style.width  = ( CFG_TITLEBAR_MIDDLE_WIDTH - this._getScrollWidth() ) + "px";
			with(this.hLabel = appendChild(document.createElement("DIV")))
			{
				className    = "MenuTitle" + fixString;
				style.width  = (CFG_TITLEBAR_MIDDLE_WIDTH - this._getScrollWidth()) + "px";
				style.height = (CFG_TITLEBAR_HEIGHT) + "px";
				style.lineHeight = (CFG_TITLEBAR_HEIGHT) + "px";
			}
			this.hLabel.reflectClass = this;		
		}
		middleCell.reflectClass = this;
		with(this.hArrayCell = titlebarRow.insertCell(2))
		{
			className   = "MenuTitlebarRight" + ((this.state == TaskMenuState.close)? "_Close" : "_Open") + fixString;
			style.width = CFG_TITLEBAR_RIGHT_WIDTH + "px";
		}
		this.hArrayCell.reflectClass = this;
		this.setLabel(this.getLabel());
		this.titlebar.appendChild(titlebarContainer);
		document.body.appendChild(this.titlebar);

		with(this.menuBody = document.createElement("DIV"))
		{
			className     = "MenuBody"  + fixString;
			style.cssText = "border-width:0px " + CFG_MENUBODY_BORDER_WIDTH + "px " + CFG_MENUBODY_BORDER_WIDTH + "px " + CFG_MENUBODY_BORDER_WIDTH + "px; " + ( (this.background != null)? "background-image:url('" + this.background + "');" : "" );
			style.top     = ( parseInt(this.titlebar.style.top) + CFG_TITLEBAR_HEIGHT ) + "px";
			style.left    = CFG_DOCUMENT_MARGIN + "px";
			style.width   = ( CFG_MENU_WIDTH + ( (INF_IS_IEXPLORER)? 0 : - (2 * CFG_MENUBODY_BORDER_WIDTH)) - ((INF_IS_SCROLLBAR_ENABLED)? CFG_SCROLLBAR_WIDTH : 0) ) + "px";
			style.height  = "auto";
			style.padding = CFG_MENUBODY_PADDING + "px 0px";
			style.MozOpacity = 1.0;
		}
		document.body.appendChild(this.menuBody);

		if(TaskMenu.headNode == null)
			TaskMenu.headNode = this;

		else
			TaskMenu.rearNode.next = this;

		this.previous     = TaskMenu.rearNode;
		TaskMenu.rearNode = this;

		this.menuID = TaskMenu.collection.push(this) - 1;

		if(this.items.length > 0)
		{
			this._setMenuFixed(false);
			this._appendItems();
			this._setMenuFixed(true);
		}
		this._setMenuHeight(this._getMenuHeight());

		if(this.state == TaskMenuState.close)
		{
			this._fixedMenuPosWhileClose();
		}
		TaskMenu._refreshAll();
	}

	this.add  = function(taskMenuItems,iPosition)
	{
		var nItem = new Array();
		if(taskMenuItems instanceof Array)
		{
			for(var i = 0; i < taskMenuItems.length; i++)
			{
				if(taskMenuItems[i] instanceof TaskMenuItem)
				{
					(dstItem = (taskMenuItems[i].container == null)? taskMenuItems[i] : taskMenuItems[i].cloneNode()).container = this;
					nItem.push(dstItem);
				}
			}
		}
		else if(taskMenuItems instanceof TaskMenuItem)
		{			
			(nItem[0] = (taskMenuItems.container == null)? taskMenuItems : taskMenuItems.cloneNode()).container = this;
		}
		else 
			return;

		if(iPosition == null || typeof iPosition != "number" || iPosition < 0 || iPosition > nItem.Length)
		{
			iPosition = this.items.length;
		}			
		this.items = this.items.slice(0,iPosition).concat( nItem , this.items.slice(iPosition ) );	

		this._setMenuFixed(true);
		this._clearMenu();
		this._appendItems();
		this._invalidate();
	}

	this.remove = function(arg1,arg2)
	{	
		var deletedItems = new Array();
		if(arguments.length == 2 && typeof arg1 == "number" && typeof arg2 == "number")
		{	
			deletedItems = this.items.splice(arg1,arg2);
		}	
		else if(arguments.length == 1 && typeof arg1 == "number")
		{
			deletedItems = this.items.splice(arg1,1);
		}
		else if(arguments.length == 1 && arg1 instanceof TaskMenuItem)
		{
			for(var i = 0; i < this.items.length; i++)
			{
				if(this.items[i] == arg1)
				{
					deletedItems = this.items.splice(i,1);	
					break;
				}
			}
		}
		else if(arguments.length == 0)
		{
			this.clear();
		}		
		else
		{
			alert("Error at TaskMenu.remove");
			return;
		}
		for(var i = 0; i < deletedItems.length; i++)
		{
			deletedItems[i].container = null;
		}
		this._clearMenu();
		this._appendItems();
		this._invalidate();
	}

	this.item = function(n)
	{
		if(typeof n == "number")
			return this.items[n];
	}

	this.clear = function()
	{
		clearTimeout(this.transformTimer);
		this._removeState(TaskMenuState.transforming);
		this._clearItem();
		this._clearMenu();
		this._invalidate();
	}

	this.setLabel = function(label)
	{
		this.label = (label == null)? new String() : label.toString();
		with(this.hLabel)
		{
			(INF_IS_IEXPLORER)? innerText = this.label : textContent = this.label;
		}
		this.titlebar.title = this.label;
	}

	this.getLabel = function()
	{
		return this.label;
	}

	this.setBackground = function(srcImage)
	{
		this.background = srcImage;
		if(this.titlebar != null && this.menuBody != null)
		{
			if(srcImage != null)
				this.menuBody.style.backgroundImage = "url(" + srcImage + ")";
			else
				this.menuBody.style.backgroundImage = "url()";
		}		
	}

	this.invalidate = function()
	{	
		if(this.titlebar == null || this.menuBody == null) return;
		
		var nScrollHeight = this._getMenuHeight();
		var tfDistance    = nScrollHeight - this.menuBody.offsetHeight;	
		
		if(tfDistance == 0) return;
		clearTimeout(this.transformTimer);
		clearTimeout(this.invalidateTimer);
		this._attachState(TaskMenuState.transforming);

		this.transformSleep = CFG_SLEEP_TIME_BEGIN;
		this.transformStep  = (tfDistance > 0)? Math.ceil(tfDistance / CFG_FRAME_COUNT) : Math.floor(tfDistance / CFG_FRAME_COUNT);

		if(this.state & TaskMenuState.closing)
			this.operationStep = Math.ceil(nScrollHeight / CFG_FRAME_COUNT);

		if(tfDistance > 0)
			this.invalidateTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._extend()",50);
		
		else
			this.invalidateTimer = setTimeout("TaskMenu.collection[" + this.menuID + "]._shorten()",50);
	}
	this.click = function()
	{
		clearTimeout(this.operationTimer);
		this.operationSleep = CFG_SLEEP_TIME_BEGIN;

		var nodeTypeFix = (this._isSpecialHeadNode())? "_Head" : "";
		this.operationStep = Math.ceil(this._getMenuHeight() / CFG_FRAME_COUNT);

		if(this.state & TaskMenuState.open || this.state & TaskMenuState.opening)
		{
			this._attachState(TaskMenuState.closing);
			this.hArrayCell.className = "MenuTitlebarRight_Close_MouseOver" + nodeTypeFix;
			this._close();
		}
		else if(this.state & TaskMenuState.close ||this.state & TaskMenuState.closing)
		{
			this._attachState(TaskMenuState.opening);
			this.hArrayCell.className = "MenuTitlebarRight_Open_MouseOver" + nodeTypeFix;
			this._open();
		}
	}
}

TaskMenu.headNode    = null;
TaskMenu.rearNode    = null;
TaskMenu.collection  = new Array();

TaskMenu._isScrollbarEnabled = function()
{	
	return (TaskMenu.rearNode.menuBody.offsetTop + TaskMenu.rearNode.menuBody.offsetHeight) > document.body.clientHeight;
}

TaskMenu._isScrollbarChanged = function()
{
	var currentScrollbarStatus;
	try
	{
		currentScrollbarStatus = TaskMenu._isScrollbarEnabled();
		return INF_IS_SCROLLBAR_ENABLED != currentScrollbarStatus;
	}
	catch(ex)
	{
		return false;
	}
	finally
	{
		INF_IS_SCROLLBAR_ENABLED = currentScrollbarStatus;
	}		
}

TaskMenu._onClick = function()
{
	var srcElement = JS.getSourceElement(arguments[0]);
	var srcMenu = srcElement.reflectClass;
	if(srcMenu != null)
	{
		srcMenu.click();
	}	
}

TaskMenu._onMouseBehavior = function()
{
	var refMenu = JS.getSourceElement(arguments[0]).reflectClass;

	var nodeTypeFix = (refMenu._isSpecialHeadNode())? "_Head" : "";
	var behaviorFix = (arguments[0].type.toUpperCase() == "MOUSEOVER")? "_MouseOver" : "";

	refMenu.hLabel.className = "MenuTitle" + behaviorFix + nodeTypeFix;
	if( (refMenu.state & TaskMenuState.open) || (refMenu.state & TaskMenuState.opening) )
	{
		refMenu.hArrayCell.className = "MenuTitlebarRight_Open" + behaviorFix + nodeTypeFix;
	}
	else if( (refMenu.state & TaskMenuState.close) || (refMenu.state & TaskMenuState.closing) )
	{
		refMenu.hArrayCell.className = "MenuTitlebarRight_Close" + behaviorFix + nodeTypeFix;	
	}		
}

TaskMenu._invalidateAll = function()
{
	for(var i = 0; i < TaskMenu.collection.length; i++)
	{
		TaskMenu.collection[i]._fixMenuWidth();
	}
}

TaskMenu._refreshAll = function()
{
	if( CFG_IS_SCROLLBAR_ENABLED )
		if(TaskMenu._isScrollbarChanged())
		{	
			TaskMenu._invalidateAll();
		}
}	

TaskMenu.setStyle = function(strCssFile)
{
	var HeadElement = document.getElementsByTagName("HEAD")[0];
	var cssFileID   = "__TaskMenuCssFile__";
	while(document.getElementById("TaskMenuCssFile") != null)
	{
		try
		{	
			HeadElement.removeChild(document.getElementById(cssFileID ));	
		}		
		catch(ex)	
		{}
	}
	with(HeadElement.appendChild(document.createElement("LINK")))
	{
		id = cssFileID; href = strCssFile; rel = "stylesheet"; type = "text/css";
	}
}

TaskMenu.setAutoBehavior = function(value)
{
	INF_IS_MENU_BEHAVIOR_AUTO = (value == false)? false : true;
}

TaskMenu.setHeadMenuSpecial = function(value)
{
	CFG_IS_SPECIAL_HEAD_NODE = (value == true)? true : false;
}

TaskMenu.setScrollbarEnabled = function(value)
{
	CFG_IS_SCROLLBAR_ENABLED = (value == false)? false : true;
	try
	{
		if(CFG_IS_SCROLLBAR_ENABLED)
			document.body.style.overflowY = "auto";

		else
			document.body.style.overflowY = "hidden";
	}
	catch(ex)
	{
		alert(ex.description);
	}
}

function JS()
{}
JS.getSourceElement = function(evt)
{
	return (evt.target == null)? evt.srcElement : evt.target;
}
JS.addListener = function(obj,eventName,callbackFunction,flag)
{ 
	if(obj.attachEvent)
		obj.attachEvent("on" + eventName,callbackFunction);
	else if(obj.addEventListener)
		obj.addEventListener(eventName,callbackFunction,flag);
	else
		eval("obj.on" + eventName + "=" + callbackFunction);
}
JS.removeListener = function(obj,eventName,callbackFunction,flag)
{ 
	if(obj.detachEvent)
		obj.detachEvent("on" + eventName,callbackFunction);
	else if (obj.removeEventListener)
		obj.removeEventListener(eventName,callbackFunction,flag);
	else
		eval("obj.on" + eventName + "= new Function()");
}

JS.addListener(window,"resize",new Function("setTimeout(\"TaskMenu._refreshAll()\",30);"),false);
JS.addListener(window,"error",new Function("return true"),false);
