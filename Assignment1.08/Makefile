CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -ggdb -funroll-loops -fpermissive
CXXFLAGS = -ggdb -funroll-loops -fpermissive                               
LDFLAGS = -lncurses

BIN = rlg327
OBJS = rlg327.o heap.o dungeon.o path.o utils.o character.o event.o \
       pc.o npc.o move.o io.o descriptions.o dice.o object.o monster.o

all: $(BIN)

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm