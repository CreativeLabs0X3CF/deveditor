import sys
from xml.dom import minidom

def main(argv):
    x = minidom.parse(open(argv[0]))

    y = x.childNodes[0]

    l = []
    for z in y.childNodes:
        if z.nodeType != z.TEXT_NODE:
            l.append((z.childNodes[1].childNodes[0].attributes.items()[0][1], z.childNodes[1].childNodes[0].childNodes[0].data))

    print "\n".join([str(e) for e in l])
    #<section ref="./stdstring/index.html" title="Standard C String &amp; Character" />
    #<keyword ref="./stdio/index.html">Standard C I/O</keyword>

    for e in l:
        print " " * 12 + "<section ref=\"%s%s\" title=\"%s\">" % (argv[1], e[0], e[1])
        if len(argv) <= 2:
            print " " * 16 + "<keyword ref=\"%s%s\">%s</keyword>" % (argv[1], e[0], e[1])
        else:
            print " " * 16 + "<keyword ref=\"%s%s\">%s (%s)</keyword>" % (argv[1], e[0], e[1], argv[2])
        print " " * 12 + "</section>"

if __name__ == "__main__":
    main(sys.argv[1:])