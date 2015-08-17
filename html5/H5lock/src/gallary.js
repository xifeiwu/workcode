function showPic(whichimg){
	var source = whichimg.getAttribute("href");
	var placeholder = document.getElementById("placeholder");
	placeholder.setAttribute("src", source);
}
