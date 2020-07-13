Tested on a 2.2 GHz Intel Core i7. Time is in ms.

The table below compares the sorting function for the C++ `std::list` to
a similar sorting function used with the `List` structure.

| N        | C++ List Random | C List Random |
|----------|-----------------|---------------|
|     100  | 0.02900         | 0.04300       |
|    1000  | 0.28200         | 0.27800       |
|   10000  | 0.82700         | 2.66300       |
|  100000  | 11.33400        | 11.37800      |
| 1000000  | 214.58200       | 209.87600     |
| 10000000 | 4569.80600      | 4498.11900    |
