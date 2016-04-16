# source directories and files
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INSTALLROOT = /usr/local

OUT    = $(BINDIR)/asn1dump


C_FILES   := $(wildcard $(SRCDIR)/*.c)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))
 
# C++ compiler, flags
CC = gcc
CFLAGS = -g -O2 -Wall
INCLUDES = -I./src  -I/usr/local/include
 
# Linker paths, flags
LIBS = -L$(INSTALLROOT)/lib -lm -lakasn1lib
LDFLAGS = -g

# Other commands
MKDIR_P = mkdir -p
RMDIR   = rmdir
 
.SUFFIXES: .c
.PHONY: clean directories install
 

default: directories $(OUT) 
 
.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OBJ_FILES): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
 
$(OUT): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES) $(LIBS)

# Install

install: $(OUT)
	cp $(OUT) $(INSTALLROOT)/bin

# Clean

clean:
	-rm -f $(OBJ_FILES) $(OUT)
	-${RMDIR} $(OBJDIR) $(BINDIR)


# Create directories

directories: ${OBJDIR} ${BINDIR} 

${OBJDIR}:
	${MKDIR_P} ${OBJDIR}

${BINDIR}:
	${MKDIR_P} ${BINDIR}

