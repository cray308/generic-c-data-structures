#!/usr/bin/env python3
import os
import subprocess

TestsToRun = {
    "./bin/c/benchmark_c_ds": {"ARRAY": "CVEC", "LIST": "CLIST", "QSORT": "QSORTARR"},
    "./bin/cpp/benchmark_cpp_ds": {"ARRAY": "CPPVEC", "LIST": "CPPLIST"}
}

def get_graph_data():
    nums = [10000,20000,30000,40000,50000,60000,70000,80000,90000]
    for i in range(100000, 10000001, 100000):
        nums.append(i)
    output = {"CLIST": [], "CPPLIST": [], "CVEC": [], "CPPVEC": [], "QSORTARR": []}
    for n in nums:
        for name in TestsToRun:
            mappings = TestsToRun[name]
            for ds in mappings:
                s = os.popen(f"{name} -d {ds} -n {n}")
                output[mappings[ds]].append(float(s.readlines()[-1]))
    print("\nGRAPH DATA:\n")

    for k in output:
        print(f"\n\n{k}\n")
        times = []
        for val in output[k]:
            times.append(format(val, ".3f"))
        print(",".join(times))

def get_averages():
    nums = [100, 1000, 10000, 100000, 1000000, 10000000]
    output = {"CLIST": [], "CPPLIST": [], "CVEC": [], "CPPVEC": [], "QSORTARR": []}
    for n in nums:
        for name in TestsToRun:
            mappings = TestsToRun[name]
            for ds in mappings:
                total = 0.0
                for i in range(10):
                    s = os.popen(f"{name} -d {ds} -n {n}")
                    total += float(s.readlines()[-1])
                output[mappings[ds]].append(total / 10)
    print("\n\nAVERAGES:\n")

    outputStr = f'| {"N":<10} | {"C List":<10} | {"C++ List":<10} |\n'
    outputStr += f"|-{'-' * 10}-|-{'-' * 10}-|-{'-' * 10}-|\n"
    for i in range(len(nums)):
        outputStr += f"| {nums[i]:>10} | {output['CLIST'][i]:10.3f} | {output['CPPLIST'][i]:10.3f} |\n"
    print(outputStr)
    print("\n\n")

    outputStr = f'| {"N":<10} | {"Qsort":<10} | {"C Array":<10} | {"C++ Vector":<10} |\n'
    outputStr += f"|-{'-' * 10}-|-{'-' * 10}-|-{'-' * 10}-|-{'-' * 10}-|\n"
    for i in range(len(nums)):
        outputStr += f"| {nums[i]:>10} | {output['QSORTARR'][i]:10.3f} | {output['CVEC'][i]:10.3f} | {output['CPPVEC'][i]:10.3f} |\n"
    print(outputStr)


if __name__ == "__main__":
    get_graph_data()
    get_averages()
