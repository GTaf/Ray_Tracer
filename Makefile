all:
	g++ main.cpp basics.cpp phongReflectionModel.cpp -L/usr/X11R6/lib -lm -lpthread -g -lX11 -o Ray_Tracer

mpi:
	mpic++ mainMPI.cpp basics.cpp phongReflectionModel.cpp -L/usr/X11R6/lib -lm -lpthread -g -lX11 -o Ray_TracerMPI
clean:
	rm Ray_Tracer Ray_TracerMPI
