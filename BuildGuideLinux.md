# Linux - How to build PEditor #
These are the steps to take when building PEditor on Linux.

## Dependencies ##
First check that you have all of the following packages installed.

Necessary packages:
  * Qt-4.3 http://www.trolltech.com

## Build ##
These are generic build instructions. Execute them in the top-level source directory (i.e. peditor/):

```
$ qmake
$ make
# make install
```

Note that the last step should be executed as root.

## What's happened ##
This should (assuming there were no errors) have compiled the source, and copied the executable to /usr/local/bin/. If you want to uninstall it, and no longer have the source - and can't run make uninstall, just delete /usr/local/bin/peditor.

'''Good luck'''