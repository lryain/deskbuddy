MATEOS_BUILD_TOOL_VER:=1.1.0-r04


MATEOS_ARM_TOOLS_ROOT:=~/.lrya/mateos-sdk/dist/$(MATEOS_BUILD_TOOL_VER)

MATEOS_ARM_TOOLS_BIN:=$(MATEOS_ARM_TOOLS_ROOT)/prebuilt/bin

# Compilation Tools

CROSS_COMPILE = $(MATEOS_ARM_TOOLS_BIN)/arm-oe-linux-gnueabi-

CFLAGS = -c -fPIC -Wall


# All the c files in this directory will be compiled
SRCDIR = src
# Look for includes here
INCDIR = inc
# .o and other temp files go here
OBJDIR = .output

# the library we are building
LIB = libcore.a


SRCS = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


all: $(MATEOS_ARM_TOOLS_ROOT) $(LIB)

$(LIB): $(OBJECTS)
	$(CROSS_COMPILE)ar rcs $(LIB) $(OBJECTS)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CROSS_COMPILE)clang $(CFLAGS) $< -I$(INCDIR) -o $@


clean:
	-rm -f $(LIB) $(OBJECTS)
