CC = g++
CFLAGS = -Wall -O3 --std=c++11

Exercise_01.1.exe : Exercise_01.1.o random.o datablocker.o
	$(CC) random.o datablocker.o Exercise_01.1.o -o Exercise_01.1.exe

Exercise_01.1.o : Exercise_01.1.cpp random.h datablocker.h lib.h
	$(CC) -c Exercise_01.1.cpp -o Exercise_01.1.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

datablocker.o : datablocker.cpp datablocker.h
	$(CC) -c datablocker.cpp -o datablocker.o $(CFLAGS)

clean :
	rm *.o Exercise_01.1.exe seed.out