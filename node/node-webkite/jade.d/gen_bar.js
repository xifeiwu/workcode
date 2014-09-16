var jade = require('jade'),
    fs = require('fs');

var gen_bar = jade.compile([
    '- each item, i in sequence',
    '  - if (i != sequence.length - 1)',
    '    li(data-path="#{item.path}")',
    '      a(href="#") #{item.name}',
    '      span.divider /',
    '  - else',
    '    li.active(data-path="#{item.path}")',
    '      a(href="#") #{item.name}',
].join('\n'));

result = [
{"name":"","path":""},
{"name":"home","path":"/home"},
{"name":"cos","path":"/home/cos"},
{"name":"Templates","path":"/home/cos/Templates"},
{"name":"node-webkit-v0.9.2-linux-ia32_and_example","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example"},
{"name":"file-explorer","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer"}
];
function gen_address_bar(dirs){
    var result = [];
    for(var i=0; i<dirs.length; i++){
        if(i != dirs.length-1){
            result.push('<li data-path="' + dirs[i].path + '"><a href="#">' + dirs[i].name + '</a><span class="divider">/</span></li>');
        }else{
            result.push('<li data-path="' + dirs[i].path + '"  class="active"><a href="#">' + dirs[i].name + '</a></li>');        
        }
    }
    return result.join('\n');
}
//console.log(gen_bar({ sequence: result }));
//console.log(gen_address_bar(result));
var gen_files_view = jade.compile([
    '- each file in files',
    '  .file(data-path="#{file.path}")',
    '    .icon',
    '      img(src="icons/#{file.type}.png")',
    '    .name #{file.name}',
].join('\n'));
function gen_files_view2(files){
    var result = [];
    for(var i=0; i<files.length; i++){
        result.push('<div class="file" data-path="' + files[i].path + '">');
        result.push('<div class="icon"> <img src="icons/' + files[i].type + '.png"></div>');
        result.push('<div class="name">' + files[i].name + '</div>');
        result.push('</div>');
    }
    return result.join('\n');
}
files = [
{"name":"bootstrap","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/bootstrap","type":"folder"},
{"name":"bundle.js","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/bundle.js","type":"blank"},
{"name":"icons","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/icons","type":"folder"},
{"name":"images","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/images","type":"folder"},
{"name":"index.html","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/index.html","type":"html"},
{"name":"js","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/js","type":"folder"},
{"name":"main.js","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/main.js","type":"blank"},
{"name":"node_modules","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/node_modules","type":"folder"},
{"name":"package.json","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/package.json","type":"blank"},
{"name":"README.md","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/README.md","type":"text"},
{"name":"style.css","path":"/home/cos/Templates/node-webkit-v0.9.2-linux-ia32_and_example/file-explorer/style.css","type":"css"}
];
console.log(gen_files_view(files));
