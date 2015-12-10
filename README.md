neurowm
=======

*neurowm* is an extremely configurable, fast, dynamic tiling window manager for X. It tries to be similar to *xmonad*, where you must write you own configuration using **C language** instead of Haskell language.


Requirements
============

In order to build *neurowm*, you need the Xlib header files.


Optional Dependencies
======================

If you want to run *neurowm* with all its features, install these packages:

 - **gmrun**: default application runner
 - **dzen2**: default system info panels
 - **xdotool**: default X11 command line


Installation
============

Once downloaded, `cd` to its root folder and compile the source using the **Makefile**:

	make

Now install the headers, library and binary using the **install target** (you must be root):

	sudo make install


Configuration
=============

In order to run *neurowm*, you must firstly write your own **neurowm.c** configuration file in **~/.neurowm** folder. If the folder does not exist, create it. There are examples of **neurowm.c** configuration files under the **examples** folder.

The **default neurowm configuration** lies within the config module, so, to run *neurowm* with its default parameters, you just need to load the already defined `NeuroConfigConfiguration` structure. Here is an example of the simpliest neurowm.c:

	#include <neuro/neurowm.h>

	int main() {
		return NeuroWmRun(&NeuroConfigConfiguration);
	}

Once you have written your own neurowm.c file, compile it using the `--recompile` flag:

	neurowm --recompile

If you are running *neurowm*, you can also compile and reload the configuration file on the fly without restarting X by just pressing the default `mod+q` key binding.


Running neurowm
===============

Add the following line to your **.xinitrc**:

	exec /usr/bin/neurowm


Wiki
====

Check the wiki for more help: https://github.com/nnoell/neurowm/wiki
