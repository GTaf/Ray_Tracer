all:
	g++ main.cpp basics.cpp phongReflectionModel.cpp -L/usr/X11R6/lib -lm -lpthread -lX11 -o Ray_Tracer
	
clean:
	rm Ray_Tracer
