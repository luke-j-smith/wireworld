# stdout
This version of the Wireworld program prints 1,000 iterations of a valid circuit to the terminal.

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
The compiler used in the *makefile* is GCC (https://gcc.gnu.org/) and you will need to have this installed in order to use it.
