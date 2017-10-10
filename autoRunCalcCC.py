import subprocess
import sys

outFile = sys.argv[1]

open(outFile, "w").close()

threads = ["2", "4", "6", "8", "10", "12", "14", "16"]
threadsV = [2, 4, 6, 8, 10, 12, 14, 16]

args = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./serial_CC", "graphV20000.txt"]

f = open(outFile, "a+")
f.write("Serial:  " )
f.close()
output = subprocess.check_output(args).split()



args = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./parallel_CC", "graphV20000.txt", ""]

args1 = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./parallel_CC_globalSum", "graphV20000.txt", ""]

for thread in threads:

    args[8] = thread
    args1[8] = thread

    f = open(outFile, "a+")
    f.write("Threads: " + str(thread) + "\n")
    f.close()

    f = open(outFile, "a+")
    f.write("\tLocal Sum: " + "\t")
    f.close()
    output = subprocess.check_output(args).split()

    f = open(outFile, "a+")
    f.write("\tGlobal Sum: " + "\t")
    f.close()
    output = subprocess.check_output(args1).split()
