var deviceListeners = new Array();
function addDeviceListener(cb){
    deviceListeners.push(cb);
}
function removeDeviceListener(cb){
    for(index in deviceListeners){
    console.log(deviceListeners[index]);
    if(deviceListeners[index] == cb)
        deviceListeners.splice(index, 1);
    }
}
function callDeviceListener(args){
    for(index in deviceListeners){
        deviceListeners[index](args);
    }
}
function a(){console.log('in function a', arguments);}
function b(){console.log('in function b', arguments);}
function c(){console.log('in function c', arguments);}
function d(){console.log('in function d', arguments);}
addDeviceListener(a);
addDeviceListener(b);
addDeviceListener(c);
console.log(deviceListeners);
//deviceListeners.forEach(function(value){console.log('value: ' + value);});
callDeviceListener('aaa');
removeDeviceListener(a);
console.log(deviceListeners);
