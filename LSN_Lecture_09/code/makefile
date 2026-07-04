CC = C:/msys64/ucrt64/bin/g++.exe
CFLAGS = -O3 --std=c++14 -D_USE_MATH_DEFINES
AFLAGS = -larmadillo -lopenblas
all: Exercise_09.1.exe
Exercise_09.1.exe : Exercise_09.1.o random.o
	$(CC) random.o Exercise_09.1.o -o Exercise_09.1.exe $(AFLAGS)
Exercise_09.1.o : Exercise_09.1.cpp population.h chromosome.h city.h lib.h
	$(CC) -c Exercise_09.1.cpp -o Exercise_09.1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o Exercise_09.1.exe
remove:
	rm ../OUTPUT/*.*
	rm ../OUTPUT/CONFIG/*.*