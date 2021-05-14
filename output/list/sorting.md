Tested on a 2.2 GHz Intel Core i7. Time is in ms.

The table below compares the sorting function for the C++ `std::list` to
a similar sorting function used with the `List` structure.

| N          | C List     | C++ List   |
|------------|------------|------------|
|        100 |      0.006 |      0.006 |
|       1000 |      0.046 |      0.057 |
|      10000 |      0.639 |      0.758 |
|     100000 |      9.680 |     10.946 |
|    1000000 |    190.259 |    214.372 |
|   10000000 |   4082.150 |   4556.461 |
