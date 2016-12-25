CC      := gcc
EXE     := bin\tsalis.exe

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))
DEPS    := $(patsubst %.o,%.d,$(OBJECTS))

LFLAGS  := -L..\..\libs\salis\lib -lsalis -lpdcurses
INCLUDE := -Iinclude -I..\..\libs\salis\include
CFLAGS  := -c -ggdb -MMD -Wall -Wextra -std=c89 -pedantic-errors \
           -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
#CFLAGS  := -c -O3 -DNDEBUG -MMD -Wall -Wextra -std=c89 -pedantic-errors \
#           -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(EXE)

-include $(DEPS)

$(OBJECTS):
	$(CC) $(INCLUDE) $(CFLAGS) $(patsubst build/%.o,src/%.c,$@) -o $@

clean:
	del /q build\*
	del /q $(EXE)
