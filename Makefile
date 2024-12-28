CC_LINUX = gcc
CC_WIN = x86_64-w64-mingw32-gcc

TARGET_WIN = BrickBreaker.exe
TARGET_LINUX = BrickBreaker

SRC = brick_breaker.c


PKGS=raylib

CFLAGS=-Wall -Wextra -ggdb -pedantic -Os -std=c11 `pkg-config --cflags --static $(PKGS)`

CFLAGS_WIN=-Wall -Wextra -pedantic -Os -std=c11 -static
INCLUDES_WIN = -I./raylib/src/
LDFLAGS_WIN = -L./raylib/src -L/nix/store/yhd18k3brx12zrsq0kmk4di32jk1zp41-mcfgthread-i686-w64-mingw32-1.6.1/bin

LIBS_WIN = -lraylib -lopengl32 -lgdi32 -lwinmm


LIBS=`pkg-config --libs --static $(PKGS)`

all: $(TARGET_WIN) $(TARGET_LINUX)

$(TARGET_LINUX): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET_LINUX) $(SRC) $(LIBS)

$(TARGET_WIN): $(SRC)
		$(CC_WIN) $(CFLAGS_WIN) $(INCLUDES_WIN) $(LDFLAGS_WIN) $(SRC) $(LIBS_WIN) -o $(TARGET_WIN)



win: $(TARGET_WIN)

linux: $(TARGET_LINUX)

clean:
	rm -f $(TARGET_WIN) $(TARGET_LINUX)

