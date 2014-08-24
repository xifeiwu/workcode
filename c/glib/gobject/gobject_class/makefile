CC = gcc
all:
	$(CC) -c go-hello.c `pkg-config --cflags glib-2.0 gobject-2.0`
	$(CC) -c main.c `pkg-config --cflags glib-2.0 gobject-2.0`
	$(CC) -o binary go-hello.o main.o `pkg-config --libs glib-2.0 gobject-2.0`
