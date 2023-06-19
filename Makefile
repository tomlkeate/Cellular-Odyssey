all: cellular-odyssey

run: cellular-odyssey
	make
	./cellular-odyssey

cellular-odyssey: main.c game.c game.h draw.c draw.h field.c field.h filehandler.c filehandler.o config.h 
	gcc -Ofast -std=gnu99  main.c game.c draw.c field.c filehandler.c -lncurses -o cellular-odyssey

clean:
	rm -f cellular-odyssey *.o
