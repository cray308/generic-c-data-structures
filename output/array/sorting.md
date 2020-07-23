Tested on a 2.2 GHz Intel Core i7. Time is in ms.

The table below compares running `std::sort` on a C++ `std::vector` to
running introsort and `qsort` on an `Array` structure.

| N        | C++ Vector Random | C Array Random | Qsort Random  |
|----------|-------------------|----------------|---------------|
|     100  | 0.012             | 0.014          | 0.026         |
|    1000  | 0.141             | 0.138          | 0.254         |
|   10000  | 1.293             | 1.713          | 3.022         |
|  100000  | 4.717             | 5.742          | 8.417         |
| 1000000  | 56.111            | 54.734         | 96.186        |
| 10000000 | 641.967           | 625.817        | 1126.608      |
