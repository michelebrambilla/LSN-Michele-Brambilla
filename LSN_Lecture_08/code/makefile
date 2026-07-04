CC = g++
CFLAGS = -Wall -O3 --std=c++11

Exercise_08.2.exe : Exercise_08.2.o random.o datablocker.o
	$(CC) random.o datablocker.o Exercise_08.2.o -o Exercise_08.2.exe

Exercise_08.2.o : Exercise_08.2.cpp random.h datablocker.h lib.h
	$(CC) -c Exercise_08.2.cpp -o Exercise_08.2.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

datablocker.o : datablocker.cpp datablocker.h
	$(CC) -c datablocker.cpp -o datablocker.o $(CFLAGS)

clean :
	rm *.o Exercise_08.2.exe seed.out