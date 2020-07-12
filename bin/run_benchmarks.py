#!/usr/bin/env python3
import os
import subprocess

NELEM = (100, 1000, 10000, 100000, 1000000, 10000000)
TYPES = ("ARRAY", "LIST")

Results = {
    "ARRAY": {
        "100": {}, "1000": {}, "10000": {}, "100000": {}, "1000000": {}, "10000000": {}
    },
    "LIST": {
        "100": {}, "1000": {}, "10000": {}, "100000": {}, "1000000": {}, "10000000": {}
    }
}

def test_sorting():
    for t in TYPES:
        for n in NELEM:
            s = os.popen("./bin/benchmark_sorting -d {} -n {} -r".format(t, n))
            last = s.readlines()[-1]
            Results[t][str(n)]["rand"] = float(last)
            s = os.popen("./bin/benchmark_sorting -d {} -n {}".format(t, n))
            last = s.readlines()[-1]
            Results[t][str(n)]["rev"] = float(last)


if __name__ == "__main__":
    test_sorting()
    for k in Results:
        print("\n\n{}\n".format(k))
        print("|          | RANDOM     | REVERSED   |")
        for n in Results[k]:
            print("| {:>8} | {:10.8f} | {:10.8f} |".format(n, Results[k][n]["rand"], Results[k][n]["rev"]))
