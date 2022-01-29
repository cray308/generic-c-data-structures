OPTIMIZE = 2
CVERS=c99

COMMON_FLAGS = -Wall -Wextra -Werror -Wpedantic -Wconversion
CFLAGS = -std=$(CVERS) -Iinclude -O$(OPTIMIZE) $(COMMON_FLAGS) -Wstrict-prototypes

SCAN_FILES = src/scan/deque.c src/scan/stack.c src/scan/queue.c \
 src/scan/array.c src/scan/list.c \
 src/scan/avltree.c src/scan/set.c src/scan/map.c \
 src/scan/unordered_set.c src/scan/unordered_map.c
SCAN_OBJS = $(SCAN_FILES:.c=.o) src/scan/str_out

TEST_BINARIES = bin/c/test_deque bin/c/test_stack bin/c/test_queue \
 bin/c/test_array bin/c/test_str bin/c/test_list \
 bin/c/test_avltree bin/c/test_set bin/c/test_map \
 bin/c/test_unordered_set bin/c/test_unordered_map

BENCHMARK_BINARIES = bin/c/benchmark_c_ds bin/cpp/benchmark_cpp_ds

.SECONDARY: $(SCAN_FILES)

all: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

debug: OPTIMIZE = 0
debug: CVERS=c89
debug: CFLAGS += -g -DDEBUG
debug: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

scan: CFLAGS += -g -fanalyzer
scan: $(SCAN_OBJS)

test: $(TEST_BINARIES)
	@bash bin/run_tests.sh

benchmark: $(BENCHMARK_BINARIES)
	@python3 bin/run_benchmarks.py

src/scan/%.c: tests/test_%.c include/%.h
	gcc -E -P -Iinclude -DDEBUG -D__CDS_SCAN -D__clang_analyzer__ $< -o $@
	@bash bin/expand_macros.sh $@

src/scan/str_out: tests/test_str.c include/str.h src/str.c
	gcc $(CFLAGS) -DDEBUG $< src/str.c -o $@

src/scan/%.o: src/scan/%.c
	gcc -c $(CFLAGS) $< -o $@

bin/c/test_unordered_%: tests/test_unordered_%.c include/unordered_%.h
	gcc $(CFLAGS) -o $@ $< src/hash.c

bin/c/test_str: tests/test_str.c include/str.h src/str.c
	gcc $(CFLAGS) -o $@ $< src/str.c

bin/c/test_%: tests/test_%.c include/%.h
	gcc $(CFLAGS) -o $@ $<

bin/c/benchmark_%: tests/benchmark_%.c include/array.h include/list.h
	gcc $(CFLAGS) -o $@ $<

bin/cpp/%: tests/%.cpp
	g++ -std=c++11 -O2 $(COMMON_FLAGS) -o $@ $<


clean:
	rm -f $(TEST_BINARIES) $(BENCHMARK_BINARIES) $(SCAN_OBJS) $(SCAN_FILES)
