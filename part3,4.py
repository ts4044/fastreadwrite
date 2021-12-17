from subprocess import call
import sys

testfile = str(sys.argv[1])
with open("part3,4.txt") as f:
    lines = f.readlines()

    for line in lines:
        bs,bc = line.strip().split(",")
        call(["sync"])
        with open('/proc/sys/vm/drop_caches', 'w') as stream:
            stream.write('3\n')
        print(bs,bc)
        call(["./run3", testfile, bs, bc])
