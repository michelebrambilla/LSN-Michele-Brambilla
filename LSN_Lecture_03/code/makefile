CC = g++
CFLAGS = -Wall -O3 --std=c++14 -D_USE_MATH_DEFINES

Exercise_03.1.exe : Exercise_03.1.o random.o datablocker.o
	$(CC) random.o datablocker.o Exercise_03.1.o -o Exercise_03.1.exe

Exercise_03.1.o : Exercise_03.1.cpp random.h datablocker.h functions.h
	$(CC) -c Exercise_03.1.cpp -o Exercise_03.1.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

datablocker.o : datablocker.cpp datablocker.h
	$(CC) -c datablocker.cpp -o datablocker.o $(CFLAGS)

clean :
	rm *.o Exercise_03.1.exe seed.out