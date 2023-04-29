a.out: main.o Expression.o
	g++ main.o Expression.o -fsanitize=address

main.o: main.cpp Expression.h
	g++ -c -g main.cpp -fsanitize=address

Expression.o: Expression.cpp Expression.h
	g++ -c -g Expression.cpp -fsanitize=address

clean:
	-rm *.o
	-rm a.out

