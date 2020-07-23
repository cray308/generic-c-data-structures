Tested on a 2.2 GHz Intel Core i7. Time is in ms.

The table below compares the sorting function for the C++ `std::list` to
a similar sorting function used with the `List` structure.

| N        | C++ List Random | C List Random |
|----------|-----------------|---------------|
|     100  | 0.029           | 0.030         |
|    1000  | 0.258           | 0.247         |
|   10000  | 0.827           | 2.632         |
|  100000  | 11.334          | 9.842         |
| 1000000  | 214.582         | 190.615       |
| 10000000 | 4569.80600      | 4127.574      |
