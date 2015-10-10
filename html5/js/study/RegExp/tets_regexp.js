function test1() {
  var file="test.html";
  var method = file.match(/\.([^.]+)$/)[1];
  console.log(method);
}

function test2() {
  var file="file:///home/cos/Documents/Acadine/workcode/html5/js/BOM/Grammer/grammer.js";
  var matches = file.match()
}

function test3() {
  var reg_str = "\\[bc\\]at";
  console.log(reg_str);
  var pattern = new RegExp("\\[bc\\]at", "gi");
  console.log(pattern.toString());
}

function test4() {
  var text = "this has been a short summer";
  var pattern = /(.)hort/g;
  if(pattern.test(text)) {
    console.log(RegExp.input);
    console.log(RegExp.leftContext);
    console.log(RegExp.rightContext);
    console.log(RegExp.lastMatch);
    console.log(RegExp.lastParen);
    console.log(RegExp.multiline);
  }
}
//test4();

function global_match() {
  var text = "cat, bat, sat, fat";
  var pattern = /.at/g;
  var matches = pattern.exec(text);
  console.log(matches.index);
  console.log(matches[0]);
  console.log(pattern.lastIndex);
  var matches = pattern.exec(text);
  console.log(matches.index);
  console.log(matches[0]);
  console.log(pattern.lastIndex);
}
//global_match();

function get_catch_pattern() {
  var text = "file:///home/cos/Documents/Acadine/workcode/html5/js/BOM/Grammer/grammer.js";
  var matches = text.match(/\/([a-zA-Z]*\.js)$/);
  console.log(matches.index);
  console.log(matches[0]);
  console.log(matches[1]);
}
//get_catch_pattern();
