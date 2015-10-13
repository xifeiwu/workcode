(function(exports) {
  var Mytag = document.registerElement('my-tag');
  document.body.appendChild(new Mytag());
  var mytag = document.getElementsByTagName("my-tag")[0];
  mytag.textContent = "I am a my-tag element.";
})(window);
