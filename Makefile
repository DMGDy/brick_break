PKGS=raylib 

CFLAGS=-Wall -Wextra -ggdb -pedantic -std=c11 `pkg-config --cflags --static $(PKGS)`

LIBS=`pkg-config --libs --static $(PKGS)`

brick-breaker: brick_breaker.c
	$(CC) $(CFLAGS) -o brick-breaker brick_breaker.c $(LIBS)
