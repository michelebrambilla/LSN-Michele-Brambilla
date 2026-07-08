CC = g++
CFLAGS = -Wall -O3 --std=c++11

Exercise_02.2.exe : Exercise_02.2.o random.o datablocker.o
	$(CC) random.o datablocker.o Exercise_02.2.o -o Exercise_02.2.exe

Exercise_02.2.o : Exercise_02.2.cpp random.h datablocker.h RandomWalker.h
	$(CC) -c Exercise_02.2.cpp -o Exercise_02.2.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

datablocker.o : datablocker.cpp datablocker.h
	$(CC) -c datablocker.cpp -o datablocker.o $(CFLAGS)

clean :
	rm *.o Exercise_02.2.exe seed.out