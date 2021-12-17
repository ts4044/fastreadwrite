from subprocess import call

with open("blocksize.txt") as f:
    lines = f.readlines()
    for line in lines:
        blocksize = line.strip()
        print(blocksize)
        call(["sync"])
        with open('/proc/sys/vm/drop_caches', 'w') as stream:
            stream.write('3\n')
        call(["dd", "if=testfile", "of=/dev/null", "bs="+blocksize])
        call(["dd", "if=testfile", "of=/dev/null", "bs="+blocksize])
        print("\n")
        