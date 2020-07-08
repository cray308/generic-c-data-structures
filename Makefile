CC = gcc
LD = gcc
OPTIMIZE = 2
LIBS = -lds

CFLAGS = -g -std=gnu99 -Iinclude -O$(OPTIMIZE)
CFLAGS += -Wall -Werror -Wstrict-prototypes
CFLAGS += -funsigned-char

LDFLAGS = -Llib

DEFAULTS_OBJ = include/defaults.h.gch
HEADERS = $(wildcard include/*.h)
SOURCES = $(wildcard src/*.c)
OBJS = $(SOURCES:.c=.o)
LIBRARY = lib/libds.so
TEST_BINARIES = bin/test_stack bin/test_queue bin/test_list bin/test_array
TEST_BINARIES += bin/test_rbtree bin/test_str bin/test_map bin/test_set

.SECONDARY: $(OBJS) $(TEST_OBJS)

all: $(DEFAULTS_OBJ) $(LIBRARY) $(TEST_BINARIES)

%.h.gch: %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(DEFAULTS_OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $^


bin/%: src/tests/%.c $(LIBRARY)
	$(CC) $(CFLAGS) -fPIC $(LDFLAGS) -o $@ $< $(LIBS)


test: $(TEST_BINARIES)
	@bash bin/run_tests.sh


clean:
	rm -f $(OBJS) $(LIBRARY) $(TEST_BINARIES)

#.PRECIOUS: %.o
