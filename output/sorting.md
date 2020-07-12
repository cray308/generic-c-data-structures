Tested on a 2.2 GHz Intel Core i7. Time is in ms, the average of 5 calls to
the sorting function.

The table below compares the sorting function for the C++ `std::list` to the
mergesort function for this library's `List` structure. The C sorting function
is roughly 2x slower than the C++ counterpart; in the future, I may optimize it. I
suspect at least one bottleneck is the final merge in which `prev` pointers must
be restored.

| N        | C++ List Random | C++ List Reversed | C List Random | C List Reversed |
|----------|-----------------|-------------------|---------------|-----------------|
|     100  | 0.02900         | 0.01900           | 0.00400       | 0.00200         |
|    1000  | 0.28200         | 0.16400           | 0.05300       | 0.01900         |
|   10000  | 0.82700         | 0.80600           | 1.42400       | 0.43800         |
|  100000  | 11.33400        | 4.09700           | 16.60800      | 4.39800         |
| 1000000  | 214.58200       | 44.90600          | 468.37400     | 91.26600        |
| 10000000 | 4569.80600      | 492.59700         | 9768.22300    | 1074.95100      |
