CC = /usr/bin/g++
CFLAGS = 
LIBS = -lm

sim: main.o rv.o event.o
	$(CC) $(CFLAGS) -o sim main.o rv.o event.o

main.o: rv.h event.h main.cpp
	$(CC) $(CFLAGS) -c main.cpp

rv.o: rv.h rv.cpp
	$(CC) $(CFLAGS) -c rv.cpp

event.o: event.h event.cpp
	$(CC) $(CFLAGS) -c event.cpp

clean: 
	rm -f *.o sim
