# An explanation #
PEditor is a source code editor. Some might even go as far as to say it's an IDE. What it certainly isn't is a compiler.

PEditor is used to write the code, the compiler is used to turn the code into something a computer can work with.

Hence, to actually write programmes, you'll also need a compiler. I recommend GCC, because it adheres to the latest standards, it's free and easy to get.

# Instructions #
The following steps describe the procedure for downloading and installing MinGW (the name GCC goes by on Windows). If you already have GCC or some other compiler installed, skip this section.

  1. Download: [MinGW Installer](http://heanet.dl.sourceforge.net/sourceforge/mingw/MinGW-5.1.3.exe)
  1. Run it and follow the steps. When it asks you what components you wish to install, in the combobox, select custom install and make sure you have checked: MinGW Base Tools, g++ compiler and MinGW make.
  1. Complete the installation process, noting the installation path (i.e. C:\MinGW)

Note: For obscure reasons, you may have to run the installer several times to finish the install. You'll no it's finished when it tells you it's finished. Otherwise it may have just crashed.

Now you have a compiler, all that remains is to tell PEditor about it.

# Configuring PEditor #
This is easy:

  1. Start PEditor.
  1. In the PEditor menu, click Configure compiler. The configuration dialog should pop up.
  1. In the C Compiler box, write the installation path followed by "\bin\gcc". So if you installed in C:\MinGW, you would write C:\MinGW\bin\gcc.
  1. In the C++ Compiler box, write the installation path followed by "\bin\g++". So if you installed in C:\MinGW, you would write C:\MinGW\bin\g++.
  1. Click done.

Congratulations, everything should be working now.