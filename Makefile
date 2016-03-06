#-----------------------------------------------------------------------------------------------------------------------
# CONFIGURE (Only edit this section)
#-----------------------------------------------------------------------------------------------------------------------

# Version and Name
PKG_VERSION = 0.32
PKG_NAME = neurowm
PKG_MYNAME = my${PKG_NAME}

# Build options (can be one of the following)
#  -DNDEBUG -DNXRANDR -> Without multi-head support (release build)
#  -DNDEBUG -DXRANDR  -> With multi-head support, needs libxrandr library (release build)
#  -DDEBUG -DNXRANDR  -> Without multi-head support (debug build)
#  -DDEBUG -DXRANDR   -> With multi-head support, needs libxrandr library (debug build)
PKG_BUILD_OPTIONS = -DNDEBUG -DNXRANDR
#PKG_BUILD_OPTIONS = -DNDEBUG -DXRANDR
#PKG_BUILD_OPTIONS = -DDEBUG -DNXRANDR
#PKG_BUILD_OPTIONS = -DDEBUG -DXRANDR

# Link options (<empty> | -lXrandr)
#  <empty>  -> Only if building with -DNXRANDR
#  -lXrandr -> Only if building with -DXRANDR
PKG_LINK_OPTIONS =
#PKG_LINK_OPTIONS = -lXrandr

# Compiler (cc | clang)
#  cc    -> GNU C Compiler
#  clang -> clang front-end c compiler
CC = cc
#CC = clang


#-----------------------------------------------------------------------------------------------------------------------
# DEFAULT VALUES
#-----------------------------------------------------------------------------------------------------------------------

# Compiler flags
DFLAGS = ${PKG_BUILD_OPTIONS} -DPKG_VERSION=\"${PKG_VERSION}\" -DPKG_NAME=\"${PKG_NAME}\" -DPKG_MYNAME=\"${PKG_MYNAME}\"
CFLAGS = -ggdb3 -Wall -fpic -O3 ${DFLAGS}\
         -Wextra -Wformat=2 -Werror -Wfatal-errors -Wpedantic -pedantic-errors -Wwrite-strings -Winit-self\
         -Wcast-align -Wpointer-arith -Wstrict-aliasing=2 -Wmissing-declarations -Wmissing-include-dirs -Wcast-qual\
         -Wno-unused-parameter -Wuninitialized -Wold-style-definition -Wstrict-prototypes -Wmissing-prototypes\
         -Wno-missing-braces -Wno-missing-field-initializers -Wswitch-default -Wswitch-enum -Wbad-function-cast\
         -Wstrict-overflow=5 -Winline -Wundef -Wnested-externs -Wshadow -Wunreachable-code -Wfloat-equal\
         -Wredundant-decls
LDADD = -lX11 ${PKG_LINK_OPTIONS} -pthread
LDADDTEST = -lX11 ${PKG_LINK_OPTIONS} -pthread -lcunit

# Mod names
MOD_NAMES = wm config dzen event rule workspace layout client core system geometry type theme action monitor

# Source names
SOURCE_BIN_NAME = main.c
SOURCE_NEUROWM_TEST_NAME = ${PKG_NAME}_test.c
SOURCE_CUNIT_TEST_NAME = cunit_test.c

# Object names
OBJECT_BIN_NAME = main.o
OBJECT_NEUROWM_TEST_NAME = ${PKG_NAME}_test.o
OBJECT_CUNIT_TEST_NAME = cunit_test.o

# Target names
TARGET_BIN_NAME = ${PKG_NAME}
TARGET_STATIC_LIB_NAME = lib${TARGET_BIN_NAME}.a
TARGET_SHARED_LIB_NAME = lib${TARGET_BIN_NAME}.so.${PKG_VERSION}
TARGET_SHARED_LNK_NAME = lib${TARGET_BIN_NAME}.so
TARGET_NEUROWM_TEST_NAME = ${PKG_MYNAME}_test
TARGET_CUNIT_TEST_NAME = cunit_test

# Source directories
SOURCE_DIR = src
SOURCE_NEURO_DIR = ${SOURCE_DIR}/neuro
SOURCE_TEST_DIR = ${SOURCE_DIR}/test

# Target directories
TARGET_DIR = build
TARGET_OBJ_DIR = ${TARGET_DIR}/obj
TARGET_LIB_DIR = ${TARGET_DIR}/lib
TARGET_BIN_DIR = ${TARGET_DIR}/bin

# Install names
INSTALL_MOD_NAME = neuro
INSTALL_LDCONF_NAME = ${INSTALL_MOD_NAME}.conf
INSTALL_MAN_NAME = ${TARGET_BIN_NAME}.1
INSTALL_THEME_NAME = nnoell

# Install directories
INSTALL_BIN_DIR = /usr/bin
INSTALL_LIB_DIR = /usr/lib/${INSTALL_MOD_NAME}
INSTALL_HDR_DIR = /usr/include/${INSTALL_MOD_NAME}
INSTALL_LDCONF_DIR = /etc/ld.so.conf.d
INSTALL_MAN_DIR = /usr/local/man/man1
INSTALL_THEME_DIR = /usr/share/themes

# Objects and Headers
OBJS = $(addprefix ${TARGET_OBJ_DIR}/, $(addsuffix .o, ${MOD_NAMES}))
HDRS = $(addprefix ${SOURCE_NEURO_DIR}/, $(addsuffix .h, ${MOD_NAMES}))

# Clean
CLEANEXTS = ${SOURCE_NEURO_DIR}/*~ ${SOURCE_TEST_DIR}/*~ ${SOURCE_DIR}/*~ ${SOURCE_TEST_DIR}/*~ ${OBJS} \
            ${TARGET_LIB_DIR}/${TARGET_STATIC_LIB_NAME} ${TARGET_LIB_DIR}/${TARGET_SHARED_LIB_NAME} \
            ${TARGET_LIB_DIR}/${TARGET_SHARED_LNK_NAME} ${TARGET_BIN_DIR}/${TARGET_BIN_NAME} \
            ${TARGET_OBJ_DIR}/${OBJECT_BIN_NAME} ${TARGET_OBJ_DIR}/${OBJECT_NEUROWM_TEST_NAME} \
            ${TARGET_OBJ_DIR}/${OBJECT_CUNIT_TEST_NAME} ${TARGET_BIN_DIR}/${TARGET_NEUROWM_TEST_NAME} \
            ${TARGET_BIN_DIR}/${TARGET_CUNIT_TEST_NAME}


#-----------------------------------------------------------------------------------------------------------------------
# BUILDING
#-----------------------------------------------------------------------------------------------------------------------

all: static_lib shared_lnk main test

test: neurowm_test cunit_test

obj: ${OBJS}

# build/obj/*.o: src/neuro/*.c
${TARGET_OBJ_DIR}/%.o: ${SOURCE_NEURO_DIR}/%.c
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "Compiling $<"

# build/obj/main.o: src/main.c
${TARGET_OBJ_DIR}/${OBJECT_BIN_NAME}: ${SOURCE_DIR}/${SOURCE_BIN_NAME}
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "Compiling $<"

# build/obj/neurowm_test.o: src/test/neurowm_test.c
${TARGET_OBJ_DIR}/${OBJECT_NEUROWM_TEST_NAME}: ${SOURCE_TEST_DIR}/${SOURCE_NEUROWM_TEST_NAME}
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "Compiling $<"

# build/obj/cunit_test.o: src/test/cunit_test.c
${TARGET_OBJ_DIR}/${OBJECT_CUNIT_TEST_NAME}: ${SOURCE_TEST_DIR}/${SOURCE_CUNIT_TEST_NAME}
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "Compiling $<"

main: ${OBJS} ${TARGET_OBJ_DIR}/${OBJECT_BIN_NAME}
	@${CC} ${CFLAGS} -o ${TARGET_BIN_DIR}/${TARGET_BIN_NAME} ${TARGET_OBJ_DIR}/${OBJECT_BIN_NAME} ${OBJS} ${LDADD}
	@echo "Linking   ${TARGET_BIN_DIR}/${TARGET_BIN_NAME}"

neurowm_test: ${OBJS} ${TARGET_OBJ_DIR}/${OBJECT_NEUROWM_TEST_NAME}
	@${CC} ${CFLAGS} -o ${TARGET_BIN_DIR}/${TARGET_NEUROWM_TEST_NAME} ${TARGET_OBJ_DIR}/${OBJECT_NEUROWM_TEST_NAME} ${OBJS} ${LDADDTEST}
	@echo "Linking   ${TARGET_BIN_DIR}/${TARGET_NEUROWM_TEST_NAME}"

cunit_test: ${OBJS} ${TARGET_OBJ_DIR}/${OBJECT_CUNIT_TEST_NAME}
	@${CC} ${CFLAGS} -o ${TARGET_BIN_DIR}/${TARGET_CUNIT_TEST_NAME} ${TARGET_OBJ_DIR}/${OBJECT_CUNIT_TEST_NAME} ${OBJS} ${LDADDTEST}
	@echo "Linking   ${TARGET_BIN_DIR}/${TARGET_CUNIT_TEST_NAME}"

static_lib: obj
	@ar -cq ${TARGET_LIB_DIR}/${TARGET_STATIC_LIB_NAME} ${OBJS}
	@echo "Creating  ${TARGET_LIB_DIR}/${TARGET_STATIC_LIB_NAME}"

shared_lib: obj
	@${CC} -shared -o ${TARGET_LIB_DIR}/${TARGET_SHARED_LIB_NAME} ${OBJS}
	@echo "Creating  ${TARGET_LIB_DIR}/${TARGET_SHARED_LIB_NAME}"

shared_lnk: shared_lib
	@ln -s -r -f ${TARGET_LIB_DIR}/${TARGET_SHARED_LIB_NAME} ${TARGET_LIB_DIR}/${TARGET_SHARED_LNK_NAME}
	@echo "Creating  ${TARGET_LIB_DIR}/${TARGET_SHARED_LNK_NAME}"

clean:
	@rm -f ${CLEANEXTS}


#-----------------------------------------------------------------------------------------------------------------------
# INSTALL / UNINSTALL
#-----------------------------------------------------------------------------------------------------------------------

# Install no bin
install_no_bin:
	@echo -n ":: Installing headers...   "
	@mkdir -p ${INSTALL_HDR_DIR}
	@install ${HDRS} ${INSTALL_HDR_DIR}
	@echo "OK"
	@echo -n ":: Installing libraries...   "
	@mkdir -p ${INSTALL_LIB_DIR}
	@install -s ${TARGET_LIB_DIR}/${TARGET_STATIC_LIB_NAME} ${INSTALL_LIB_DIR}
	@install -s ${TARGET_LIB_DIR}/${TARGET_SHARED_LIB_NAME} ${INSTALL_LIB_DIR}
	@ln -s -r -f ${INSTALL_LIB_DIR}/${TARGET_SHARED_LIB_NAME} ${INSTALL_LIB_DIR}/${TARGET_SHARED_LNK_NAME}
	@mkdir -p ${INSTALL_LDCONF_DIR}
	@echo ${INSTALL_LIB_DIR} > ${INSTALL_LDCONF_DIR}/${INSTALL_LDCONF_NAME}
	@ldconfig
	@echo "OK"
	@echo -n ":: Installing man page...   "
	@mkdir -p ${INSTALL_MAN_DIR}
	@install man/${INSTALL_MAN_NAME} ${INSTALL_MAN_DIR}
	@chmod 644 ${INSTALL_MAN_DIR}/${INSTALL_MAN_NAME}
	@echo "OK"
	@echo -n ":: Installing themes...   "
	@mkdir -p ${INSTALL_THEME_DIR}
	@cp -r themes/${INSTALL_THEME_NAME} ${INSTALL_THEME_DIR}
	@echo "OK"

# Install
install: install_no_bin
	@echo -n ":: Installing binary...   "
	@install -s ${TARGET_BIN_DIR}/${TARGET_BIN_NAME} ${INSTALL_BIN_DIR}
	@echo "OK"

# Uninstall no bin
uninstall_no_bin:
	@echo -n ":: Uninstalling headers...   "
	@rm -rf ${INSTALL_HDR_DIR}
	@echo "OK"
	@echo -n ":: Uninstalling libraries...   "
	@rm -rf ${INSTALL_LIB_DIR}
	@rm -f ${INSTALL_LDCONF_DIR}/${INSTALL_LDCONF_NAME}
	@ldconfig
	@echo "OK"
	@echo -n ":: Uninstalling man page...   "
	@rm -f ${INSTALL_MAN_DIR}/${INSTALL_MAN_NAME}
	@echo "OK"
	@echo -n ":: Uninstalling themes...   "
	@rm -rf ${INSTALL_THEME_DIR}/${INSTALL_THEME_NAME}
	@echo "OK"

# Uninstall
uninstall: uninstall_no_bin
	@echo -n ":: Uninstalling binary...   "
	@rm -f ${INSTALL_BIN_DIR}/${TARGET_BIN_NAME}
	@echo "OK"
