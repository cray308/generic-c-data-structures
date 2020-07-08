CC = gcc
LD = gcc
OPTIMIZE = 2
LIBS = -lds

CFLAGS = -g -Iinclude -O$(OPTIMIZE)
CFLAGS += -Wall -Werror -Wstrict-prototypes
CFLAGS += -funsigned-char

LDFLAGS = -Llib

DEFAULTS_OBJ = include/defaults.h.gch
HEADERS = $(wildcard include/*.h)
C89_SOURCES = src/ds.c src/stack.c src/queue.c src/array.c src/list.c src/rbtree.c
C89_SOURCES += src/map.c src/set.c
C99_SOURCES = src/str.c
SOURCES = $(C89_SOURCES) $(C99_SOURCES)
C89_OBJS = $(C89_SOURCES:.c=.o)
C99_OBJS = $(C99_SOURCES:.c=.o)
OBJS = $(SOURCES:.c=.o)
LIBRARY = lib/libds.so
TEST_BINARIES = bin/test_stack bin/test_queue bin/test_list bin/test_array
TEST_BINARIES += bin/test_rbtree bin/test_str bin/test_map bin/test_set

.SECONDARY: $(OBJS)

all: $(DEFAULTS_OBJ) $(LIBRARY) $(TEST_BINARIES)

%.h.gch: %.h
	$(CC) $(CFLAGS) -std=c99 -c $< -o $@

$(LIBRARY): $(C89_OBJS) $(C99_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $^

$(C89_OBJS): EXTRA_FLAGS := -std=c89

$(C99_OBJS): EXTRA_FLAGS := -std=c99

%.o: %.c $(DEFAULTS_OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(EXTRA_FLAGS) -fPIC -c $< -o $@


test: $(TEST_BINARIES)
	@bash bin/run_tests.sh

bin/%: src/tests/%.c $(LIBRARY)
	$(CC) $(CFLAGS) -std=c99 -fPIC $(LDFLAGS) -o $@ $< $(LIBS)


clean:
	rm -f $(OBJS) $(LIBRARY) $(TEST_BINARIES)

#.PRECIOUS: %.o
