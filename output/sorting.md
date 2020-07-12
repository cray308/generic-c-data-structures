Tested on a 2.2 GHz Intel Core i7. Time is in ms, the average of 5 calls to
the sorting function.

The table below compares `Array`, which uses `qsort`, to a mergesort function
for `List`.

| N        | Array Random | Array Reversed | List Random | List Reversed |
|----------|--------------|----------------|-------------|---------------|
|     100  | 0.00400      | 0.00200        | 0.00400     | 0.00200       |
|    1000  | 0.05000      | 0.01600        | 0.05300     | 0.01900       |
|   10000  | 0.64400      | 0.18400        | 1.42400     | 0.43800       |
|  100000  | 7.95800      | 2.07200        | 16.60800    | 4.39800       |
| 1000000  | 94.47500     | 23.30900       | 468.37400   | 91.26600      |
| 10000000 | 1111.29200   | 282.32700      | 9768.22300  | 1074.95100    |
