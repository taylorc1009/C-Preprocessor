This is a preprocess application (command line/prompt).

To access the program, first (in the command window) type "nmake clean" to remove remove any files used
from  the previous use then type "nmake" on its own.

Next, type "preprocess -i (filename.c)" to run the application. You can also add "-c" after the file name
to remove the comments when creating an output (.o) file.

This program was built using Visual Studio 2017 and can be compiled and ran using the VS developer
command line.

Sources:
https://stackoverflow.com/questions/12733105/c-function-that-counts-lines-in-file
http://www.zentut.com/c-tutorial/c-file-exists/
http://www.cplusplus.com/reference/cstdio/fseek/
