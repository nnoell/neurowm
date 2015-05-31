#-----------------------------------------------------------------------------------------------------------------------
# CONFIG
#-----------------------------------------------------------------------------------------------------------------------

# Version
VERSION = 0.12.1

# Compiler flags
CC = gcc
CPPFLAGS = -DVERSION=\"${VERSION}\"
#CFLAGS = -Wall -Wextra -O3 ${CPPFLAGS}
#CFLAGS = -Wall -Wextra -Wformat -Wfatal-errors -O3 ${CPPFLAGS}
#CFLAGS = -Wall -Wextra -Wformat -Werror -Wfatal-errors -O3 ${CPPFLAGS}
CFLAGS = -Wall -Wextra -Wformat -Werror -Wfatal-errors -Wpedantic -pedantic-errors -O3 ${CPPFLAGS}
LDADD = -lX11 -pthread

# Names and Directories
MODSDIR = ./neuro
TESTDIR = ./test
CONFDIR = ./conf
TESTSRC = neurowm.c
TESTOUT = myneurowm
BINNAME = neurowm
MAINSRC = main.c
MODS = neurowm config dzenpanel event rule workspace layout client stackset base area general personal

# Do not change
OBJS = $(addprefix $(MODSDIR)/, $(addsuffix .o, $(MODS)))
HDRS = $(addprefix $(MODSDIR)/, $(addsuffix .h, $(MODS)))
LIBNAME = lib$(BINNAME).a
CLEANEXTS = *~ $(MODSDIR)/*~ $(OBJS) $(BINNAME) $(LIBNAME) $(TESTDIR)/*~ $(TESTDIR)/$(TESTOUT) $(CONFDIR)/*~


#-----------------------------------------------------------------------------------------------------------------------
# BUILDING
#-----------------------------------------------------------------------------------------------------------------------

# All target
all: $(OBJS) main $(LIBNAME)

# Test target
test: $(TESTDIR)/$(TESTSRC)

# Main target
main: $(MAINSRC)
	${CC} $(CFLAGS) -o $(BINNAME) $<

# Src/Obj target
.c.o:
	${CC} -c ${CFLAGS} $< -o $@

# Test link
$(TESTDIR)/$(TESTSRC): $(OBJS)
	${CC} $(CFLAGS) -o $(TESTDIR)/$(TESTOUT) $(TESTDIR)/$(TESTSRC) ${OBJS} $(LDADD)

# Lib target
$(LIBNAME):
	@ar -cvq $(LIBNAME) $(OBJS)

# Clean target
clean:
	@rm -f $(CLEANEXTS)


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
	@cp $(LIBNAME) /usr/lib/neuro
	@echo "OK"
	@echo -n ":: Copying binary to \"/usr/bin\"...   "
	@cp $(BINNAME) /usr/bin
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
