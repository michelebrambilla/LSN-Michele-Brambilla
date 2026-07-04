CC = C:/msys64/ucrt64/bin/g++.exe

MPI_INC = -I"C:/Program Files (x86)/Microsoft SDKs/MPI/Include"
MPI_LIB = -L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64" -lmsmpi

CFLAGS = -O3 --std=c++14 -D_USE_MATH_DEFINES $(MPI_INC)
AFLAGS = -larmadillo -lopenblas $(MPI_LIB)
all: Exercise_10.1.exe
Exercise_10.1.exe : Exercise_10.1.o random.o
	$(CC) random.o Exercise_10.1.o -o Exercise_10.1.exe $(AFLAGS)
Exercise_10.1.o : Exercise_10.1.cpp population.h chromosome.h city.h lib.h
	$(CC) -c Exercise_10.1.cpp -o Exercise_10.1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o Exercise_10.1.exe
remove:
	rm ../OUTPUT/*.*
	rm ../OUTPUT/CONFIG/*.*