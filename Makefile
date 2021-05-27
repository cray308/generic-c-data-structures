CFLAGS = -std=c89 -Iinclude -O2
CFLAGS += -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes
CFLAGS += -funsigned-char -finline-functions

CPPFLAGS = -std=c++11 -O2
CPPFLAGS += -Wall -Wextra -Werror -Wpedantic
CPPFLAGS += -funsigned-char -finline-functions

HEADERS = $(wildcard include/*.h)

TEST_BINARIES = bin/c/test_deque bin/c/test_stack bin/c/test_queue
TEST_BINARIES += bin/c/test_array bin/c/test_str bin/c/test_list
TEST_BINARIES += bin/c/test_avltree bin/c/test_set bin/c/test_map
TEST_BINARIES += bin/c/test_unordered_set bin/c/test_unordered_map

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
	gcc $(CFLAGS) -o $@ $<

bin/c/benchmark_%: tests/benchmark_%.c $(HEADERS)
	gcc $(CFLAGS) -o $@ $<

bin/cpp/%: tests/%.cpp
	g++ $(CPPFLAGS) -o $@ $<


clean:
	rm -f $(TEST_BINARIES) $(BENCHMARK_BINARIES)
