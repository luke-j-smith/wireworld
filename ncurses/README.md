# ncurses
This version of the Wireworld program displays 1,000 iterations of a valid circuit using ncurses in the terminal.

## Usage
This program can be compiled using the *makefile*:
```
make
```

The program can then be run using the *makefile* with the input file specified:
```
make run WIREWORLD="input_file_name.txt"
```

Three example Wireworld board have been provided - two are valid and one is invalid - they are called: ```wirewcircuit1.txt```, ```wirewcircuit2.txt``` and ```wirewinvalid.txt```.

### Requirements
The compiler used in the *makefile* is GCC (https://gcc.gnu.org/) and you will need to have this installed in order to use it. Also, due to the use of ncurses, this will only work in unix-style terminals.

## neillncurses
There are two files ```neillncurses.c``` and ```neillncurses.h``` that I have not written. These were written by Neill Campbell (http://www.bristol.ac.uk/engineering/people/neill-w-campbell/index.html) to facilitate the use of ncurses in the assignment (as the direct use of this library was not being assessed).
