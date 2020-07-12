CC = gcc
LD = gcc
OPTIMIZE = 2
LIBS = -lds -lpthread

CFLAGS = -std=c99 -Iinclude -O$(OPTIMIZE)
CFLAGS += -Wall -Wextra -Werror -Wstrict-prototypes
CFLAGS += -funsigned-char -finline-functions

LDFLAGS = -Llib

HEADERS = $(wildcard include/*.h)
SOURCES = $(wildcard src/*.c)
OBJS = $(SOURCES:.c=.o)
LIBRARY = lib/libds.so
TEST_BINARIES = bin/test_stack bin/test_queue bin/test_list bin/test_array
TEST_BINARIES += bin/test_rbtree bin/test_str bin/test_map bin/test_set

BENCHMARK_BINARIES = bin/benchmark_sorting

.SECONDARY: $(OBJS)

all: $(LIBRARY) $(TEST_BINARIES) $(BENCHMARK_BINARIES)

debug: CFLAGS += -g
debug: $(LIBRARY) $(TEST_BINARIES) $(BENCHMARK_BINARIES)

%.h.gch: %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@


test: $(TEST_BINARIES)
	@bash bin/run_tests.sh

bin/%: src/tests/%.c $(LIBRARY)
	$(CC) $(CFLAGS) -fPIC $(LDFLAGS) -o $@ $< $(LIBS)

benchmark: $(BENCHMARK_BINARIES)
	@python3 bin/run_benchmarks.py


clean:
	rm -f $(OBJS) $(LIBRARY) $(TEST_BINARIES) $(BENCHMARK_BINARIES)

#.PRECIOUS: %.o
