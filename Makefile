all:
	g++ main.cpp basics.cpp phongReflectionModel.cpp -L/usr/X11R6/lib -lm -lpthread -lX11
	
clean:
	rm a.out
