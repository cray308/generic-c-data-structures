CC = gcc
LD = gcc
OPTIMIZE = 2

CFLAGS = -std=c99 -Iinclude -O$(OPTIMIZE)
CFLAGS += -Wall -Wextra -Werror -Wstrict-prototypes
CFLAGS += -funsigned-char -finline-functions

CPPFLAGS = -std=c++11 -O$(OPTIMIZE)
CPPFLAGS += -Wall -Wextra -Werror
CPPFLAGS += -funsigned-char -finline-functions

HEADERS = $(wildcard include/*.h)

TEST_BINARIES = bin/c/test_stack bin/c/test_queue bin/c/test_list bin/c/test_array bin/c/test_str
TEST_BINARIES += bin/c/test_unordered_set bin/c/test_unordered_map
TEST_BINARIES += bin/c/test_set bin/c/test_map

BENCHMARK_BINARIES = bin/c/benchmark_c_ds bin/cpp/benchmark_cpp_ds

.SECONDARY: $(OBJS)

all: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

debug: CFLAGS += -g
debug: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

test: $(TEST_BINARIES)
	@bash bin/run_tests.sh

benchmark: $(BENCHMARK_BINARIES)
	@python3 bin/run_benchmarks.py


bin/c/test_%: tests/test_%.c include/ds.h include/%.h
	$(CC) $(CFLAGS) -o $@ $<

bin/c/benchmark_%: tests/benchmark_%.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

bin/cpp/%: tests/%.cpp
	g++ $(CPPFLAGS) -o $@ $<


clean:
	rm -f $(TEST_BINARIES) $(BENCHMARK_BINARIES)
