a.out: main.o Expression.o
	g++ main.o Expression.o

main.o: main.cpp Expression.h
	g++ -c -g main.cpp

Expression.o: Expression.cpp Expression.h
	g++ -c -g Expression.cpp

clean:
	-rm *.o
	-rm a.out

