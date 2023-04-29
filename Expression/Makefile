a.out: main.o Expression.o
	g++ -g main.o Expression.o -lgtest

main.o: main.cpp Expression.h
	g++ -c main.cpp

Expression.o: Expression.cpp Expression.h
	g++ -c Expression.cpp

clean:
	-rm *.o
	-rm a.out

