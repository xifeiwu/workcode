var os=require('os');
var ifaces=os.networkInterfaces();
var ipv4="";
for (var dev in ifaces) {
  if(dev == 'lo'){continue;}
  ifaces[dev].forEach(function(details){
    if (details.family=='IPv4') {
      ipv4 = details.address;
    }
  });
}
console.log(ipv4);
