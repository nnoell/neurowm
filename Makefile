#-----------------------------------------------------------------------------------------------------------------------
# CONFIG
#-----------------------------------------------------------------------------------------------------------------------

# Version
VERSION = 0.13

# Compiler flags
CC = gcc
DFLAGS = -DVERSION=\"${VERSION}\"
#CFLAGS = -Wall -Wextra -O3 ${DFLAGS}
#CFLAGS = -Wall -Wextra -Wformat -Wfatal-errors -O3 ${DFLAGS}
#CFLAGS = -Wall -Wextra -Wformat -Werror -Wfatal-errors -O3 ${DFLAGS}
CFLAGS = -Wall -Wextra -Wformat -Werror -Wfatal-errors -Wpedantic -pedantic-errors -O3 ${DFLAGS}
LDADD = -lX11 -pthread
LDADDTEST = -lX11 -pthread -lcunit

# Names and Directories
MODSDIR = ./neuro
TESTDIR = ./test
CONFDIR = ./conf
BUILDIR = ./build
TESTSRC = neurowm_test.c
TESTOUT = myneurowm_test
UNITSRC = cunit_test.c
UNITOUT = cunit_test
BINNAME = neurowm
MAINSRC = main.c
MODS = neurowm config dzenpanel event rule workspace layout client stackset base area general personal

# Do not change
OBJS = $(addprefix $(MODSDIR)/, $(addsuffix .o, $(MODS)))
HDRS = $(addprefix $(MODSDIR)/, $(addsuffix .h, $(MODS)))
LIBNAME = lib$(BINNAME).a
CLEANEXTS = *~ $(MODSDIR)/*~ $(OBJS) $(TESTDIR)/*~ $(TESTDIR)/$(TESTOUT) $(CONFDIR)/*~ $(BUILDIR)


#-----------------------------------------------------------------------------------------------------------------------
# BUILDING
#-----------------------------------------------------------------------------------------------------------------------

# All target
all: $(OBJS) main $(LIBNAME)

# Test target
test: $(TESTDIR)/$(TESTSRC) $(TESTDIR)/$(UNITSRC)

# Main target
main: $(MAINSRC)
	@mkdir -p $(BUILDIR)
	${CC} $(CFLAGS) -o $(BUILDIR)/$(BINNAME) $<

# Src/Obj target
.c.o:
	@mkdir -p $(BUILDIR)
	${CC} -c ${CFLAGS} $< -o $@

# Test neurowm
$(TESTDIR)/$(TESTSRC): $(OBJS)
	@mkdir -p $(BUILDIR)
	${CC} $(CFLAGS) -o $(BUILDIR)/$(TESTOUT) $(TESTDIR)/$(TESTSRC) $(OBJS) $(LDADDTEST)

# Test cunit
$(TESTDIR)/$(UNITSRC): $(OBJS)
	@mkdir -p $(BUILDIR)
	${CC} $(CFLAGS) -o $(BUILDIR)/$(UNITOUT) $(TESTDIR)/$(UNITSRC) $(OBJS) $(LDADDTEST)

# Lib target
$(LIBNAME):
	@mkdir -p $(BUILDIR)
	@ar -cvq $(BUILDIR)/$(LIBNAME) $(OBJS)

# Clean target
clean:
	@rm -rf $(CLEANEXTS)


#-----------------------------------------------------------------------------------------------------------------------
# INSTALL
#-----------------------------------------------------------------------------------------------------------------------

# Install
install:
	@echo
	@echo -n ":: Copying headers to \"/usr/include/neuro\"...   "
	@mkdir -p /usr/include/neuro
	@cp $(HDRS) /usr/include/neuro
	@echo "OK"
	@echo -n ":: Copying lib to \"/usr/lib/neuro\"...   "
	@mkdir -p /usr/lib/neuro
	@cp $(BUILDIR)/$(LIBNAME) /usr/lib/neuro
	@echo "OK"
	@echo -n ":: Copying binary to \"/usr/bin\"...   "
	@cp $(BUILDIR)/$(BINNAME) /usr/bin
	@echo "OK"
	@echo -n ":: Copying man page to \"/usr/local/man/man1\"...   "
	@mkdir -p /usr/local/man/man1
	@cp neurowm.1 /usr/local/man/man1
	@chmod 644 /usr/local/man/man1/neurowm.1
	@echo "OK"
	@echo -e "Done!\n"

# Uninstall
uninstall:
	@echo
	@echo -n ":: Removing directory \"/usr/include/neuro\"...   "
	@rm -rf /usr/include/neuro
	@echo "OK"
	@echo -n ":: Removing directory \"/usr/lib/neuro\"...   "
	@rm -rf /usr/lib/neuro
	@echo "OK"
	@echo -n ":: Removing binary \"/usr/bin/neuro\"...   "
	@rm -rf /usr/bin/neuro
	@echo "OK"
	@echo -n ":: Removing man page \"/usr/local/man/man1/neurowm.1\"...   "
	@rm -rf /usr/local/man/man1/neurowm.1
	@echo "OK"
	@echo -e "Done!\n"
