#!/usr/bin/env node
/*
<copyright>
Copyright (c) 2011, Motorola Mobility, Inc

All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  - Neither the name of Motorola Mobility nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
</copyright>
*/

var dbus = require('../dbus');
var http = require('http');

var dbusMsg = Object.create(dbus.DBusMessage, {
  destination: {
    value: 'org.freedesktop.Avahi'
  },
  path: {
    value: '/'
  },
  iface: {
    value: 'org.freedesktop.Avahi.Server'
  },
  member: {
    value: 'ServiceBrowserNew',
    writable: true
  },
  bus: {
    value: dbus.DBUS_BUS_SYSTEM
  },
  type: {
    value: dbus.DBUS_MESSAGE_TYPE_METHOD_CALL,
    writable: true
  }
});
//console.log("dbus.DBUS_SERVICE_DBUS:" + dbus.DBUS_SERVICE_DBUS);
//console.log("dbus.DBUS_PATH_DBUS:" + dbus.DBUS_PATH_DBUS);
//console.log("dbus.DBUS_INTERFACE_DBUS:" + dbus.DBUS_INTERFACE_DBUS);
var service_browser_path="";
dbusMsg.on ("methodResponse", function (arg) {
  console.log ("[PASSED] Got method response with data ::");
  //console.log (arguments);
  service_browser_path = arguments[0];
  console.log ("service_browser_path: " + service_browser_path);
  //console.log ("length of service_browser_path: " + service_browser_path.length);
  if(service_browser_path.length){
		var dbusMonitorSignal = Object.create(dbus.DBusMessage, {
		  path: {
		    value: service_browser_path,
		    writable: true
		  },
		  iface: {
		    value: 'org.freedesktop.Avahi.ServiceBrowser',
		    writable: true
		  },
		  member: {
		    value: 'ItemNew',
		    writable: true
		  },
		  bus: {
		    value: dbus.DBUS_BUS_SYSTEM,
		    writable: true
		  },
		  type: {
		    value: dbus.DBUS_MESSAGE_TYPE_SIGNAL
		  }
		});
		dbusMonitorSignal.addMatch();
		dbusMonitorSignal.addMatch(member='ItemRemove');
		dbusMonitorSignal.on ("signalReceipt", function () {
		  console.log ("[PASSED] Signal received with data :: ");
		  console.log (arguments);
		});
		dbusMonitorSignal.on ("error", function (error) {
		  console.log ("[FAILED] ERROR -- ");
		  console.log(error);
		});

  }
});

dbusMsg.on ("error", function (error) {
  console.log ("[FAILED] ERROR -- ");
  console.log(error);
});

//Asynchronous call for 'ServiceBrowserNew'
dbusMsg.appendArgs('iissu', 
							-1, -1, '_http._tcp', 'local', 0);
dbusMsg.send();

		var httpServer = http.createServer(function(){});
		httpServer.listen(8084);

//switch to synchronous call for 'ListNames' which does not expect any argument
/*
dbusMsg.type = dbus.DBUS_MESSAGE_TYPE_METHOD_CALL;
dbusMsg.clearArgs();
dbusMsg.member = 'ListNames';
dbusMsg.send();
*/
