#!/usr/bin/env python3
import os
import subprocess

NELEM = (10000,20000,30000,40000,50000,60000,70000,80000,90000,
100000,200000,300000,400000,500000,600000,700000,800000,900000,
1000000,1100000,1200000,1300000,1400000,1500000,1600000,1700000,1800000,1900000,
2000000,2100000,2200000,2300000,2400000,2500000,2600000,2700000,2800000,2900000,
3000000,3100000,3200000,3300000,3400000,3500000,3600000,3700000,3800000,3900000,
4000000,4100000,4200000,4300000,4400000,4500000,4600000,4700000,4800000,4900000,
5000000,5100000,5200000,5300000,5400000,5500000,5600000,5700000,5800000,5900000,
6000000,6100000,6200000,6300000,6400000,6500000,6600000,6700000,6800000,6900000,
7000000,7100000,7200000,7300000,7400000,7500000,7600000,7700000,7800000,7900000,
8000000,8100000,8200000,8300000,8400000,8500000,8600000,8700000,8800000,8900000,
9000000,9100000,9200000,9300000,9400000,9500000,9600000,9700000,9800000,9900000,
10000000)

Results = {
    "CLIST": {},
    "CPPLIST": {},
    "CVEC": {},
    "CPPVEC": {}
}

def test_array():
    for n in NELEM:
        s = os.popen("./bin/c/benchmark_c_ds -d ARRAY -n {}".format(n))
        last = s.readlines()[-1]
        Results["CVEC"][str(n)] = float(last)
        s = os.popen("./bin/cpp/benchmark_cpp_ds -d ARRAY -n {}".format(n))
        last = s.readlines()[-1]
        Results["CPPVEC"][str(n)] = float(last)
        print("finished {}".format(n))

def test_list():
    for n in NELEM:
        s = os.popen("./bin/c/benchmark_c_ds -d LIST -n {}".format(n))
        last = s.readlines()[-1]
        Results["CLIST"][str(n)] = float(last)
        s = os.popen("./bin/cpp/benchmark_cpp_ds -d LIST -n {}".format(n))
        last = s.readlines()[-1]
        Results["CPPLIST"][str(n)] = float(last)
        print("finished {}".format(n))


if __name__ == "__main__":
    test_array()
    test_list()
    for k in Results:
        print("\n\n{}\n".format(k))
        times = []
        for n in Results[k]:
            times.append(format(Results[k][n], '.3f'))
        print(",".join(times))
