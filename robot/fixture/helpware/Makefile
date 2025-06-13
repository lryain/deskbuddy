
# MATEOS_BUILD_TOOL_VER:=1.1.0-r04


# MATEOS_ARM_TOOLS_ROOT:=~/.lrya/mateos-sdk/dist/$(MATEOS_BUILD_TOOL_VER)

# MATEOS_ARM_TOOLS_BIN:=$(MATEOS_ARM_TOOLS_ROOT)/prebuilt/bin

# # Compilation Tools

# CROSS_COMPILE = $(MATEOS_ARM_TOOLS_BIN)/arm-oe-linux-gnueabi-
CROSS_COMPILE = ""

CFLAGS = -c -fPIC -Wall -Werror
# 添加目标架构标志
ARCH = aarch64
CFLAGS += -march=armv8-a  # 对于 Orange Pi 5 Pro


CORE_DIR = ../../core
CORE = $(CORE_DIR)/libcore.a

# Compiler flags
# Specify all the flags below which you want to use for your compilation, For this simple example, we only need to specify the include directory path
#CFLAGS          :=  -pie -I. -fPIC

# link with core lib
LIBS    += -lcore
LIBPATH += -L$(CORE_DIR)
CFLAGS  += -I../ -O2
CFLAGS  += -I$(CORE_DIR)/inc
CFLAGS  += -DMATEOS
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function

CFLAGS  += -I../../syscon
CFLAGS  += -I../../hal
CFLAGS  += -I../../../platform/gpio

# All the c files in this directory will be compiled
SRCDIR = .
# .o and other temp files go here
OBJDIR = .output

# GPIO_OBJ = $(GPIO_SRC:%.c=$(OBJDIR)/%.o)
GPIO_SRC = ../../../platform/gpio/gpio.c
GPIO_OBJ = $(OBJDIR)/gpio.o

# 编译 gpio.o
$(OBJDIR)/gpio.o: $(GPIO_SRC)
	mkdir -p $(OBJDIR)
	$(CROSS_COMPILE)clang $(CFLAGS) -c $< -o $@

DFU = dfu
DFU_SRC = dfu.c
DFU_OBJ = $(DFU_SRC:%.c=$(OBJDIR)/%.o)

ANIMATE = animate
ANIMATE_SRC = animate.c
ANIMATE_OBJ = $(ANIMATE_SRC:%.c=$(OBJDIR)/%.o)

DISPLAY = display
DISPLAY_SRC = display.c helper_text.c display_test.c
DISPLAY_OBJ = $(DISPLAY_SRC:%.c=$(OBJDIR)/%.o)

DISPLAY_BLIT = display_blit
DISPLAY_BLIT_SRC = display.c display_blit_test.c
DISPLAY_BLIT_OBJ = $(DISPLAY_BLIT_SRC:%.c=$(OBJDIR)/%.o)

DISPLAY_TEST = display_test
DISPLAY_TEST_SRC = display.c display_test.c
DISPLAY_TEST_OBJ = $(DISPLAY_TEST_SRC:%.c=$(OBJDIR)/%.o)

HELPER = helper
HELPER_SRC = main.c display.c helper_text.c logging.c pidopen.c kbhit.c
HELPER_OBJ = $(HELPER_SRC:%.c=$(OBJDIR)/%.o)

PIDOPEN = pidopen
PIDOPEN_SRC = pidopen.c
PIDOPEN_OBJ = $(PIDOPEN_SRC:%.c=$(OBJDIR)/%.o)

TESTLCD = test_lcd
TESTLCD_SRC = test_lcd.c
TESTLCD_OBJ = $(TESTLCD_SRC:%.c=$(OBJDIR)/%.o)

COMMANDER = commander
COMMANDER_SRC = commander.c kbhit.c display.c
COMMANDER_OBJ = $(COMMANDER_SRC:%.c=$(OBJDIR)/%.o) $(SPINE_OBJ)

SPINE_SRC = spine_hal.c spine_crc.c
SPINE_OBJ = $(SPINE_SRC:%.c=$(OBJDIR)/%.o)

MENUMAN = menuman
MENUMAN_SRC = menuman.c kbhit.c display.c pidopen.c
MENUMAN_OBJ = $(MENUMAN_SRC:%.c=$(OBJDIR)/%.o) $(SPINE_OBJ)

BURN = burn
BURN_SRC = burn.c
BURN_OBJ = $(BURN_SRC:%.c=$(OBJDIR)/%.o)

REMOTE_DEST = /data/local/fixture/

# all: $(MATEOS_ARM_TOOLS_BIN) $(DFU) $(ANIMATE) $(DISPLAY) $(HELPER) $(MENUMAN) $(BURN)
all: $(DFU) $(ANIMATE) $(DISPLAY) $(DISPLAY_BLIT) $(HELPER) $(MENUMAN) $(BURN) $(DISPLAY_TEST)
# all: $(DISPLAY_BLIT)

$(CORE):
	cd $(CORE_DIR) && make all

$(MATEOS_ARM_TOOLS_BIN):
	../../../tools/build/tools/lryabuild/mateos.py --install $(MATEOS_BUILD_TOOL_VER)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CROSS_COMPILE)clang $(CFLAGS) $<  -o $@


$(DFU): $(CORE) $(DFU_OBJ)
	$(CROSS_COMPILE)clang -pie $(DFU_OBJ) $(LIBPATH) $(LIBS)  -o $(DFU)

$(ANIMATE): $(CORE) $(ANIMATE_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(ANIMATE_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS)  -o $(ANIMATE)

$(DISPLAY): $(CORE) $(DISPLAY_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(DISPLAY_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS)  -o $(DISPLAY)

$(DISPLAY_BLIT): $(CORE) $(DISPLAY_BLIT_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)gcc -pie $(DISPLAY_BLIT_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS)  -o $(DISPLAY_BLIT)

$(DISPLAY_TEST): $(CORE) $(DISPLAY_BLIT_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)gcc -pie $(DISPLAY_BLIT_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS)  -o $(DISPLAY_TEST)

$(HELPER): $(CORE) $(HELPER_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(HELPER_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS)  -o $(HELPER)

$(BURN): $(CORE) $(BURN_OBJ)
	$(CROSS_COMPILE)clang -pie $(BURN_OBJ) $(LIBPATH) $(LIBS)  -o $(BURN)

$(COMMANDER): $(CORE) $(COMMANDER_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(COMMANDER_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS) -llog -o $(COMMANDER)

$(OBJDIR)/spine_hal.o:
	$(CROSS_COMPILE)clang $(CFLAGS) ../../hal/spine/spine_hal.c  -o $@
$(OBJDIR)/spine_crc.o:
	$(CROSS_COMPILE)clang $(CFLAGS) ../../hal/spine/spine_crc.c  -o $@


$(MENUMAN): $(CORE) $(MENUMAN_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(MENUMAN_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS) -llog  -o $(MENUMAN)

$(TESTLCD): $(CORE) $(TESTLCD_OBJ) $(GPIO_OBJ)
	$(CROSS_COMPILE)clang -pie $(TESTLCD_OBJ) $(GPIO_OBJ) $(LIBPATH) $(LIBS) -llog  -o $(TESTLCD)

app=$(HELPER)

push: $(app)
	adb shell -x "mkdir -p $(REMOTE_DEST)"
	adb push $(app)  $(REMOTE_DEST)

execute: $(app)
	adb shell -x "cd $(REMOTE_DEST) && ./$(app) $(safe)"

cleant:
	rm -f $(APP) $(OBJDIR)/animate.o
	rm -f $(APP) $(OBJDIR)/test_lcd.o

clean:
	rm -f $(APP) $(OBJDIR)/*.o
	rm -f $(CORE_DIR)/libcore.a
	rm -f $(DFU) $(ANIMATE) $(DISPLAY) $(DISPLAY_BLIT) $(HELPER) $(MENUMAN) $(BURN) $(COMMANDER) $(TESTLCD)
