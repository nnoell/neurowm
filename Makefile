#-----------------------------------------------------------------------------------------------------------------------
# CONFIG
#-----------------------------------------------------------------------------------------------------------------------

# Version
VERSION = 0.17.3

# Compiler flags
CC = gcc
#DFLAGS = -DVERSION=\"${VERSION}\"
DFLAGS = -DNDEBUG -DVERSION=\"${VERSION}\"
CFLAGS = -Wall -Wextra -Wformat -Werror -Wfatal-errors -Wpedantic -pedantic-errors -O3 ${DFLAGS}
LDADD = -lX11 -pthread
LDADDTEST = -lX11 -pthread -lcunit

# Mod names
MOD_NAMES = neurowm config dzenpanel event rule workspace layout client stackset system geometry type personal action

# Source names
SOURCE_BIN_NAME = main.c
SOURCE_NEUROWM_TEST_NAME = neurowm_test.c
SOURCE_CUNIT_TEST_NAME = cunit_test.c

# Object names
OBJECT_BIN_NAME = main.o
OBJECT_NEUROWM_TEST_NAME = neurowm_test.o
OBJECT_CUNIT_TEST_NAME = cunit_test.o

# Target names
TARGET_BIN_NAME = neurowm
TARGET_LIB_NAME = lib$(TARGET_BIN_NAME).a
TARGET_NEUROWM_TEST_NAME = myneurowm_test
TARGET_CUNIT_TEST_NAME = cunit_test

# Source directories
SOURCE_DIR = src
SOURCE_NEURO_DIR = $(SOURCE_DIR)/neuro
SOURCE_TEST_DIR = $(SOURCE_DIR)/test

# Target directories
TARGET_DIR = build
TARGET_OBJ_DIR = $(TARGET_DIR)/obj
TARGET_LIB_DIR = $(TARGET_DIR)/lib
TARGET_BIN_DIR = $(TARGET_DIR)/bin

# Do not change
OBJS = $(addprefix $(TARGET_OBJ_DIR)/, $(addsuffix .o, $(MOD_NAMES)))
HDRS = $(addprefix $(SOURCE_NEURO_DIR)/, $(addsuffix .h, $(MOD_NAMES)))
CLEANEXTS = $(SOURCE_NEURO_DIR)/*~ $(SOURCE_TEST_DIR)/*~ $(SOURCE_DIR)/*~ $(SOURCE_TEST_DIR)/*~ $(OBJS) \
            $(TARGET_LIB_DIR)/$(TARGET_LIB_NAME) $(TARGET_BIN_DIR)/$(TARGET_BIN_NAME) \
            $(TARGET_OBJ_DIR)/$(OBJECT_BIN_NAME) $(TARGET_OBJ_DIR)/$(OBJECT_NEUROWM_TEST_NAME) \
            $(TARGET_OBJ_DIR)/$(OBJECT_CUNIT_TEST_NAME) $(TARGET_BIN_DIR)/$(TARGET_NEUROWM_TEST_NAME) \
            $(TARGET_BIN_DIR)/$(TARGET_CUNIT_TEST_NAME)


#-----------------------------------------------------------------------------------------------------------------------
# BUILDING
#-----------------------------------------------------------------------------------------------------------------------

all: lib main test

test: neurowm_test cunit_test

obj: $(OBJS)

# build/obj/*.o: src/neuro/*.c
$(TARGET_OBJ_DIR)/%.o: $(SOURCE_NEURO_DIR)/%.c
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "Compiling $<"

# build/obj/main.o: src/main.c
$(TARGET_OBJ_DIR)/$(OBJECT_BIN_NAME): $(SOURCE_DIR)/$(SOURCE_BIN_NAME)
	@${CC} $(CFLAGS) -c -o $@ $<
	@echo "Compiling $<"

# build/obj/neurowm_test.o: src/test/neurowm_test.c
$(TARGET_OBJ_DIR)/$(OBJECT_NEUROWM_TEST_NAME): $(SOURCE_TEST_DIR)/$(SOURCE_NEUROWM_TEST_NAME)
	@${CC} $(CFLAGS) -c -o $@ $<
	@echo "Compiling $<"

# build/obj/cunit_test.o: src/test/cunit_test.c
$(TARGET_OBJ_DIR)/$(OBJECT_CUNIT_TEST_NAME): $(SOURCE_TEST_DIR)/$(SOURCE_CUNIT_TEST_NAME)
	@${CC} $(CFLAGS) -c -o $@ $<
	@echo "Compiling $<"

main: $(OBJS) $(TARGET_OBJ_DIR)/$(OBJECT_BIN_NAME)
	@${CC} $(CFLAGS) -o $(TARGET_BIN_DIR)/$(TARGET_BIN_NAME) $(TARGET_OBJ_DIR)/$(OBJECT_BIN_NAME) $(OBJS) $(LDADD)
	@echo "Linking   $(TARGET_BIN_DIR)/$(TARGET_BIN_NAME)"

neurowm_test: $(OBJS) $(TARGET_OBJ_DIR)/$(OBJECT_NEUROWM_TEST_NAME)
	@${CC} $(CFLAGS) -o $(TARGET_BIN_DIR)/$(TARGET_NEUROWM_TEST_NAME) $(TARGET_OBJ_DIR)/$(OBJECT_NEUROWM_TEST_NAME) $(OBJS) $(LDADDTEST)
	@echo "Linking   $(TARGET_BIN_DIR)/$(TARGET_NEUROWM_TEST_NAME)"

cunit_test: $(OBJS) $(TARGET_OBJ_DIR)/$(OBJECT_CUNIT_TEST_NAME)
	@${CC} $(CFLAGS) -o $(TARGET_BIN_DIR)/$(TARGET_CUNIT_TEST_NAME) $(TARGET_OBJ_DIR)/$(OBJECT_CUNIT_TEST_NAME) $(OBJS) $(LDADDTEST)
	@echo "Linking   $(TARGET_BIN_DIR)/$(TARGET_CUNIT_TEST_NAME)"

lib: obj
	@ar -cq $(TARGET_LIB_DIR)/$(TARGET_LIB_NAME) $(OBJS)
	@echo "Creating  $(TARGET_LIB_DIR)/$(TARGET_LIB_NAME)"

clean:
	@rm -f $(CLEANEXTS)


#-----------------------------------------------------------------------------------------------------------------------
# INSTALL / UNINSTALL
#-----------------------------------------------------------------------------------------------------------------------

# Install no bin
install_no_bin:
	@echo -n ":: Installing headers...   "
	@mkdir -p /usr/include/neuro
	@cp $(HDRS) /usr/include/neuro
	@echo "OK"
	@echo -n ":: Installing library...   "
	@mkdir -p /usr/lib/neuro
	@cp $(TARGET_LIB_DIR)/$(TARGET_LIB_NAME) /usr/lib/neuro
	@echo "OK"
	@echo -n ":: Installing man page...   "
	@mkdir -p /usr/local/man/man1
	@cp man/neurowm.1 /usr/local/man/man1
	@chmod 644 /usr/local/man/man1/neurowm.1
	@echo "OK"

# Install
install: install_no_bin
	@echo -n ":: Installing binary...   "
	@cp $(TARGET_BIN_DIR)/$(TARGET_BIN_NAME) /usr/bin
	@echo "OK"

# Uninstall no bin
uninstall_no_bin:
	@echo -n ":: Uninstalling headers...   "
	@rm -rf /usr/include/neuro
	@echo "OK"
	@echo -n ":: Uninstalling library...   "
	@rm -rf /usr/lib/neuro
	@echo "OK"
	@echo -n ":: Uninstalling man page...   "
	@rm -f /usr/local/man/man1/neurowm.1
	@echo "OK"

# Uninstall
uninstall: uninstall_no_bin
	@echo -n ":: Uninstalling binary...   "
	@rm -f /usr/bin/neurowm
	@echo "OK"
