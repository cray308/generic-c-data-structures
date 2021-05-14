Tested on a 2.2 GHz Intel Core i7. Time is in ms.

The table below compares running `std::sort` on a C++ `std::vector` to
running introsort and `qsort` on a C array.

| N          | Qsort      | C Array    | C++ Vector |
|------------|------------|------------|------------|
|        100 |      0.006 |      0.003 |      0.003 |
|       1000 |      0.059 |      0.031 |      0.031 |
|      10000 |      0.696 |      0.380 |      0.385 |
|     100000 |      8.482 |      4.676 |      4.736 |
|    1000000 |    101.801 |     54.857 |     55.804 |
|   10000000 |   1187.663 |    642.532 |    653.181 |
