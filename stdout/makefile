TARGET1 = wireworld

COMP = gcc
CFLAGS = -pedantic -Wall -Wextra -Wfloat-equal -ansi -O2
CLIBS = -lncurses -lm

$(TARGET1) : $(TARGET1).c

				$(COMP) $(TARGET1).c -o $(TARGET1) $(CFLAGS) $(CLIBS)

clean :
				rm -f $(TARGET1)

run :
				./$(TARGET1) $(WIREWORLD)
