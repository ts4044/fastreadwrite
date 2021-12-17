from subprocess import call

with open("blocksize.txt") as f:
    lines = f.readlines()
    for line in lines:
        blocksize = line.strip()
        call(["./run2", "testfile", blocksize])