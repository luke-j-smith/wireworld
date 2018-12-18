TARGET1 = wireworld_moving
TARGET2 = neillncurses

COMP = gcc
CFLAGS = -pedantic -Wall -Wextra -Wfloat-equal -ansi -O2
CLIBS = -lncurses -lm

$(TARGET1) : $(TARGET1).c $(TARGET2).c $(TARGET2).h

				$(COMP) $(TARGET1).c $(TARGET2).c -o $(TARGET1) $(CFLAGS) $(CLIBS)

clean :
				rm -f $(TARGET1)

run :
				./$(TARGET1) $(WIREWORLD)
