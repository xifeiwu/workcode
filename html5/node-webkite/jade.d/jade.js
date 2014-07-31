var jade = require('jade'),
    fs = require('fs');

fs.readFile('template.jade', 'utf8', function (err, data) {
    if (err) throw err;
    console.log(data);
    var fn = jade.compile(data);
    cur_dir = process.cwd();
    fs.readdir(cur_dir, function(error, files){
        for(var i=0; i<files.length; i++){
            console.log(files[i]);
        }
    });
    var html = fn({name:'Oleg'});
    console.log(html);
});
