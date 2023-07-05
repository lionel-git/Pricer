#
# Cf: https://arturtamborski.pl/2018/03/simple-generic-makefile
#
TARGET  = $(notdir $(CURDIR))

SRCEXT	= .cpp
INCEXT	= .h
OBJEXT	= .o

SRCDIR	= .
INCDIR	= .
OBJDIR	= _objs

CC	= clang++
LD	= clang++

# Add libfmt if needed
FMTLIB	= $(shell [ -d /usr/include/fmt ] && echo -lfmt)

LDFLAGS	= -lm
CCFLAGS	= -std=c++20 -g -ggdb -Og -Wall -Wextra -pedantic -Weverything -Wno-c++98-compat -Wno-padded -Werror=non-virtual-dtor

SRCTREE	= $(shell find $(SRCDIR) -type d)
INCS	= $(shell find $(INCDIR) -type f -name '*$(INCEXT)')
SRCS	= $(shell find $(SRCDIR) -type f -name '*$(SRCEXT)')
OBJTREE	= $(foreach D,$(SRCTREE),$(shell echo $(D) | sed 's/$(SRCDIR)/$(OBJDIR)/'))
OBJSTMP	= $(foreach F,$(SRCS),$(shell echo $(F) | sed -e 's/$(SRCDIR)/$(OBJDIR)/'))
OBJS	= $(foreach O,$(OBJSTMP),$(shell echo $(O) | sed -e 's/\$(SRCEXT)/\$(OBJEXT)/'))

all: $(TARGET)
	@echo Done.

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -r $(TARGET) $(OBJS) $(OBJDIR) 2>/dev/null || true

$(TARGET): $(OBJS) | $(OBJDIR)
	$(LD) $(LDFLAGS) -L$(OBJDIR) -o $@ $^

$(OBJS): $(OBJDIR)/%$(OBJEXT) : $(SRCDIR)/%$(SRCEXT) | $(OBJDIR)
	$(CC) $(CCFLAGS) -I$(INCDIR) -c -o $@ $?

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(OBJTREE)

# comment

.PHONY: all run clean
