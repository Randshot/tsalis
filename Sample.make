CC := gcc

## WINDOWS ##
EXE := bin\tsalis.exe

## UNIX ##
#EXE := bin/tsalis

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))
DEPS    := $(patsubst %.o,%.d,$(OBJECTS))

## Add correct path to SALIS library to LFLAGS (e.g. -L..\path\to\salis\lib).
## Make sure to use correct SLASH! ('/' or '\').
## Also, link correct NCurses library for your platform.
LFLAGS  := -L..\path\to\salis\lib -lsalis -lpdcurses

## Add correct path to SALIS include files (e.g. -I..path\to\salis\include).
## Make sure to use correct SLASH! ('/' or '\').
INCLUDE := -Iinclude -I..path\to\salis\include

## RELEASE flags ##
CFLAGS  := -c -O3 -DNDEBUG -MMD -Wall -Wextra -std=c89 -pedantic-errors \
           -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition

## DEBUG flags ##
#CFLAGS  := -c -ggdb -MMD -Wall -Wextra -std=c89 -pedantic-errors \
#           -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(EXE)

-include $(DEPS)

$(OBJECTS):
	$(CC) $(INCLUDE) $(CFLAGS) $(patsubst build/%.o,src/%.c,$@) -o $@

## WINDOWS ##
clean:
	del /q build\*
	del /q $(EXE)

## UNIX ##
#clean:
#	rm build\*
#	rm $(EXE)