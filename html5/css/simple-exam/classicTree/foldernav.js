/*
 ______________________________________________________
/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\
|           Folder Navigation 2.0 by EAE               |
|                                                      |
|   Based on a cross browser outline sample found      |
|   at 'www.webreference.com/dhtml'                    |
|                                                      |
|   Feel free to copy, use and change this script as   |
|   long as this part remains unchanged.               |
|                                                      |
|   If you have any questions and or comments please   |
|   E-mail me 'eae@eae.net'. If you're looking for     |
|   more JavaScripts etc, please check out my webpage  |
|                 'www.eae.net/weebfx'                 |
|                                                      |
|              Last Updated: 17 July 1998              |
\______________________________________________________/
 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
*/

document.onmouseover = mOver ;
document.onmouseout = mOut ;

function mOver() {
	var eSrc = window.event.srcElement ;
	if (eSrc.className == "item") {
		window.event.srcElement.className = "highlight";
	}
}

function mOut() {
	var eSrc = window.event.srcElement ;
	if (eSrc.className == "highlight") {
		window.event.srcElement.className = "item";
	}
}


var bV=parseInt(navigator.appVersion);
NS4=(document.layers) ? true : false;
IE4=((document.all)&&(bV>=4))?true:false;
ver4 = (NS4 || IE4) ? true : false;

isExpanded = false;

function getIndex($1) {
	ind = null;
	for (i=0; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		if (whichEl.id == $1) {
			ind = i;
			break;
		}
	}
	return ind;
}

function arrange() {
	nextY = document.layers[firstInd].pageY + document.layers[firstInd].document.height;
	for (i=firstInd+1; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		if (whichEl.visibility != "hide") {
			whichEl.pageY = nextY;
			nextY += whichEl.document.height;
		}
	}
}

function FolderInit(){
	if (NS4) {
	firstEl = "mParent";
	firstInd = getIndex(firstEl);
	showAll();
		for (i=0; i<document.layers.length; i++) {
			whichEl = document.layers[i];
			if (whichEl.id.indexOf("Child") != -1) whichEl.visibility = "hide";
		}
		arrange();
	}
	else {
		tempColl = document.all.tags("DIV");
		for (i=0; i<tempColl.length; i++) {
			if (tempColl(i).className == "child") tempColl(i).style.display = "none";
		}
	}
}

function FolderExpand($1,$2,$3) {
	if (!ver4) return;
	if (IE4) { ExpandIE($1,$2,$3) } 
	else { ExpandNS($1,$2,$3) }
}

function ExpandIE($1,$2,$3) {
	Expanda = eval($1 + "a");
	Expanda.blur()
	ExpandChild = eval($1 + "Child");
        if ($2 != "top") { 
		ExpandTree = eval($1 + "Tree");
		ExpandFolder = eval($1 + "Folder");
	}
	if (ExpandChild.style.display == "none") {
		ExpandChild.style.display = "block";
                if ($2 != "top") { 
                	if ($2 == "last") { ExpandTree.src = " Lminus.gif"; }
			else { ExpandTree.src = " Tminus.gif"; }
			ExpandFolder.src = " openfoldericon.gif";
			
		}
		else { mTree.src = " topopen.gif"; }
	}
	else {
		ExpandChild.style.display = "none";
                if ($2 != "top") { 
	                if ($2 == "last") { ExpandTree.src = " Lplus.gif"; }
			else { ExpandTree.src = " Tplus.gif"; }
			ExpandFolder.src = " foldericon.gif";
		
		}
		else { mTree.src = " topopen.gif"; }
	}
	
}
function ExpandNS($1,$2,$3) {
	ExpandChild = eval("document." + $1 + "Child")
        if ($2 != "top") { 
		ExpandTree = eval("document." + $1 + "Parent.document." + $1 + "Tree")
		ExpandFolder = eval("document." + $1 + "Parent.document." + $1 + "Folder")
	}	
	if (ExpandChild.visibility == "hide") {
		ExpandChild.visibility = "show";
                if ($2 != "top") { 
               		if ($2 == "last") { ExpandTree.src = " Lminus.gif"; }
			else { ExpandTree.src = " Tminus.gif"; }
			ExpandFolder.src = " openfoldericon.gif";	
		}
		else { mTree.src = " topopen.gif"; }
	}
	else {
		ExpandChild.visibility = "hide";
                if ($2 != "top") { 
               		if ($2 == "last") { ExpandTree.src = " Lplus.gif"; }
			else { ExpandTree.src = " Tplus.gif"; }
			ExpandFolder.src = " foldericon.gif";	
		}
		else { mTree.src = " top.gif"; }
	}
	arrange();
	
}

function showAll() {
	for (i=firstInd; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		whichEl.visibility = "show";
	}
}


with (document) {
	write("<STYLE TYPE='text/css'>");
	if (NS4) {
		write(".parent { color: black; font-size:9pt; line-height:0pt; color:black; text-decoration:none; margin-top: 0px; margin-bottom: 0px; position:absolute; visibility:hidden }");
		write(".child { text-decoration:none; font-size:9pt; line-height:15pt; position:absolute }");
	        write(".item { color: black; text-decoration:none }");
	        write(".highlight { color: blue; text-decoration:none }");
	}
	else {
		write(".parent { font: 12px/13px; Times; text-decoration: none; color: black;}");
		write(".child { font:12px/13px Times; display:none;color:black;}");
	        write(".item { color: black; text-decoration:none; cursor: hand }");
	        write(".highlight { color: blue; text-decoration:none }");
	        write(".icon { margin-right: 5 }")
		
	}
	write("</STYLE>");
}

function linew(){
with(document){
  write("<img src= I.gif align=absmiddle border='0'><img src= T.gif align=absmiddle border='0'><img src= htmlicon.gif align=absmiddle border='0'>");	
}
}

function elinew(){
with(document){
  write("<img src= white.gif align=absmiddle border='0'><img src= L.gif align=absmiddle border='0'><img src= htmlicon.gif align=absmiddle border='0'>");	
}
}

onload = FolderInit;