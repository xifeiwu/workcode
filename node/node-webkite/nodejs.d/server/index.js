var server = require("./server2");
var router = require("./router2");
var requestHandlers = require("./requestHandlers2");

var handle = {}
handle["/"] = requestHandlers.start;
handle["/start"] = requestHandlers.start;
handle["/upload"] = requestHandlers.upload;

server.start(router.route, handle);
