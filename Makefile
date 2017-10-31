FLAGS= -Wall -Werror
OBJ= obj/minicas.o obj/matrix.o obj/operations.o obj/solve.o obj/var.o obj/parser.o obj/speedtest.o obj/moindrecarre.o obj/plot.o
LIB= -lm


main: $(OBJ)
	gcc -o bin/minicas $(OBJ) $(FLAGS) $(LIB)

obj/minicas.o: src/minicas.c include/matrix.h include/operations.h include/solve.h include/var.h include/parser.h
	gcc -c src/minicas.c -o $@

obj/matrix.o: src/matrix.c include/matrix.h
	gcc -c src/matrix.c -o $@

obj/operations.o: src/operations.c include/operations.h
	gcc -c src/operations.c -o $@

obj/solve.o: src/solve.c include/solve.h include/operations.h include/matrix.h
	gcc -c src/solve.c -o $@

obj/var.o: src/var.c include/var.h include/matrix.h
	gcc -c src/var.c -o $@

obj/parser.o: src/parser.c include/parser.h include/matrix.h include/speedtest.h
	gcc -c src/parser.c -o $@

obj/speedtest.o: src/speedtest.c include/speedtest.h include/parser.h
	gcc -c src/speedtest.c -o $@

obj/moindrecarre.o: src/moindrecarre.c include/moindrecarre.h include/matrix.h include/solve.h include/operations.h include/plot.h
	gcc -c src/moindrecarre.c -o $@

obj/plot.o: src/plot.c include/plot.h include/matrix.h
	gcc -c src/plot.c -o $@

clean:
	rm obj/*.o bin/minicas
