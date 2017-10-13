import subprocess
import sys

infile = sys.argv[1]
outFile = sys.argv[2]

open(outFile, "w").close()

threads = ["2", "4","8", "16"]
threadsV = [2, 4, 8, 16]

args = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./serial_CC", infile]

f = open(outFile, "a+")
f.write("Serial:  " )
f.close()
output = subprocess.check_output(args).split()



args = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./parallel_CC", infile, ""]

args1 = ["/usr/bin/time", "-a", "-o", outFile, "-f", "%e", "./parallel_CC_globalSum", infile, ""]

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
