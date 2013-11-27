#Makefile for Ubuntu13.10
all:	main.o m3d.o xm3d.o
	clang++ -std=c++11 -o demo main.o m3d.o xm3d.o -L/usr/bin/X11 -I/usr/include/X11 -lX11
main.o:	main.cpp
	clang++ -std=c++11 -c main.cpp -I./lib/m3d -I./lib/xm3d
m3d.o:	./lib/m3d/m3d.cpp
	clang++ -std=c++11 -c ./lib/m3d/m3d.cpp
xm3d.o:	./lib/xm3d/xm3d.cpp
	clang++ -std=c++11 -c ./lib/xm3d/xm3d.cpp -I./lib/m3d
