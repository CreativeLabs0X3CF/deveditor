import sys
import os

def walkDir(d):
    files = os.listdir(d)

    for c in files:
        if c[0] == '.':
            continue
        if os.path.isdir(d + '/' + c):
            walkDir(d + '/' + c)
        else:
            print " " * 6 + "<file>%s</file>" % (d + '/' + c)

def main(argv):
    if len(argv) < 1:
        exit()

    walkDir(argv[0])

if __name__ == "__main__":
    main(sys.argv[1:])
