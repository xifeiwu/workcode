cmd_out/Default/foo := flock out/Default/linker.lock g++   -o out/Default/foo -Wl,--start-group out/Default/obj.target/foo/hello_world.o out/Default/obj.target/foo/my_class.o -Wl,--end-group 
