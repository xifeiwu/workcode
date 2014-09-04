cmd_Release/ndbus.node := ln -f "Release/obj.target/ndbus.node" "Release/ndbus.node" 2>/dev/null || (rm -rf "Release/ndbus.node" && cp -af "Release/obj.target/ndbus.node" "Release/ndbus.node")
