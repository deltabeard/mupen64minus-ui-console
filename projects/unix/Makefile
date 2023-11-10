# vi: set shiftwidth=4 tabstop=4 expandtab:
#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Mupen64plus - Makefile                                                *
# *   Mupen64Plus homepage: https://mupen64plus.org/                        *
# *   Copyright (C) 2009 Richard42                                          *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# Makefile for Mupen64plus-ui-console

define help_txt
Mupen64plus-ui-console is licensed under the terms of the GPLv2+.
Available options and their descriptions when enabled:

  Targets:
    all            == Build Mupen64Plus console front-end application (default)
    clean          == remove object files and build products
    install        == Install Mupen64Plus console front-end application
    uninstall      == Uninstall Mupen64Plus console front-end application
  Options:
    COREDIR=path   == default path to search for Mupen64Plus Core (must end with slash)
    PLUGINDIR=path == default path to search for plugins
    SHAREDIR=path  == default path to search for shared data files
    APIDIR=path    == path to find Mupen64Plus Core headers
    OPTFLAGS=flags == compiler optimization (default: -O3 -flto)
    WARNFLAGS=flag == compiler warning levels (default: -Wall)
  Install Options:
    PREFIX=path    == install/uninstall prefix (default: /usr/local/)
    BINDIR=path    == path to install mupen64plus binary (default: PREFIX/bin/)
    MANDIR=path    == path to install mupen64plus manual page (default: PREFIX/share/man)
    APPSDIR=path   == path to install desktop file (default: PREFIX/share/applications)
    ICONSDIR=path  == path to install icon files (default: PREFIX/share/icons/hicolor)
    DESTDIR=path   == path to prepend to all installation paths (only for packagers)
  Debugging Options:
    DEBUG=(1|0)    == add debugging symbols to application binary
    DEBUGGER=(1|0) == enable debugging capability

  Build options are currently set to:
    COREDIR="$(COREDIR)"
    PLUGINDIR="$(PLUGINDIR)"
    SHAREDIR="$(SHAREDIR)"
    APIDIR="$(APIDIR)"
    OPTFLAGS="$(OPTFLAGS)"
    WARNFLAGS="$(WARNFLAGS)"

    PREFIX="$(PREFIX)"
    BINDIR="$(BINDIR)"
    MANDIR="$(MANDIR)"
    APPSDIR="$(APPSDIR)"
    ICONSDIR="$(ICONSDIR)"
    DESTDIR="$(DESTDIR)"

    DEBUG=$(DEBUG)
    DEBUGGER=$(DEBUGGER)

  Example: make DEBUG=1 OPTFLAGS="-Ofast -march=native"
endef

SRCDIR = ../../src
OBJDIR = _obj

# User-configurable build parameters
DEBUGGER := 0
DEBUG := 0
OPTFLAGS := -Ofast -flto
WARNFLAGS := -Wall -Wextra -fsanitize=undefined -fsanitize-trap

# set base program pointers and flags
RM	?= rm -f
INSTALL	?= install
MKDIR	?= mkdir -p

# set installation options
PREFIX := /usr/local
LIBDIR := $(PREFIX)/lib
BINDIR := $(PREFIX)/bin
MANDIR := $(PREFIX)/share/man
APPSDIR := $(PREFIX)/share/applications
ICONSDIR := $(PREFIX)/share/icons/hicolor

ifeq ($(BIG_ENDIAN), 1)
  CFLAGS += -DM64P_BIG_ENDIAN
endif

# set mupen64plus core API header path
ifneq ("$(APIDIR)","")
  CFLAGS += "-I$(APIDIR)"
else
  TRYDIR = ../../../mupen64minus-core/src/api
  ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
    CFLAGS += -I$(TRYDIR)
  else
    TRYDIR = /usr/local/include/mupen64plus
    ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
      CFLAGS += -I$(TRYDIR)
    else
      TRYDIR = /usr/include/mupen64plus
      ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
        CFLAGS += -I$(TRYDIR)
      else
        $(error Mupen64Plus API header files not found! Use makefile parameter APIDIR to force a location.)
      endif
    endif
  endif
endif

# set special flags for given Makefile parameters
ifeq ($(PLUGINDIR),)
  CFLAGS += '-DPLUGINDIR="$(LIBDIR)/mupen64plus"'
else
  CFLAGS += '-DPLUGINDIR="$(PLUGINDIR)"'
endif
# note: COREDIR _must_ end in a slash if you want it to work; not necessary for the others
ifeq ($(COREDIR),)
  CFLAGS += '-DCOREDIR="$(LIBDIR)/"'
else
  CFLAGS += '-DCOREDIR="$(COREDIR)"'
endif
ifneq ($(SHAREDIR),)
  CFLAGS += '-DSHAREDIR="$(SHAREDIR)"'
endif
ifeq ($(DEBUG),1)
  OPTFLAGS := -g3 -O0
  INSTALL_STRIP_FLAG ?= 
else
  INSTALL_STRIP_FLAG ?= -s
endif
ifeq ($(DEBUGGER),1)
  CFLAGS += -DENABLE_DEBUGGER=1
else
  CFLAGS += -DENABLE_DEBUGGER=0
endif

# list of source files to compile
SOURCE = \
	$(SRCDIR)/cheat.c \
	$(SRCDIR)/compare_core.c \
	$(SRCDIR)/core_interface.c \
	$(SRCDIR)/main.c \
	$(SRCDIR)/plugin.c

ifeq ($(DEBUGGER), 1)
	SOURCE += $(SRCDIR)/debugger.c
endif

ifeq ($(OS),Windows_NT)
SOURCE += \
	$(SRCDIR)/osal_dynamiclib_win32.c \
	$(SRCDIR)/osal_files_win32.c
else
SOURCE += \
	$(SRCDIR)/osal_dynamiclib_unix.c \
	$(SRCDIR)/osal_files_unix.c
LDLIBS += -ldl
endif

# generate a list of object files build, make a temporary directory for them
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter %.c, $(SOURCE)))
OBJDIRS = $(dir $(OBJECTS))
$(shell $(MKDIR) $(OBJDIRS))

# build targets
TARGET = mupen64plus

# build dependency files
CFLAGS += -MD -MP
-include $(OBJECTS:.o=.d)

CFLAGS += -pthread $(OPTFLAGS) $(WARNFLAGS) -I$(SRCDIR)

all: $(TARGET)

clean:
	$(RM) -r $(OBJDIR) $(TARGET)

help:
	$(info $(help_txt))
	@cd

install: $(TARGET)
	$(INSTALL) -d "$(DESTDIR)$(BINDIR)"
	$(INSTALL) -m 0755 $(INSTALL_STRIP_FLAG) $(TARGET) "$(DESTDIR)$(BINDIR)"
	$(INSTALL) -d "$(DESTDIR)$(MANDIR)/man6"
	$(INSTALL) -m 0644 $(SRCDIR)/../doc/mupen64plus.6 "$(DESTDIR)$(MANDIR)/man6"
	$(INSTALL) -d "$(DESTDIR)$(APPSDIR)"
	$(INSTALL) -m 0644 $(SRCDIR)/../data/mupen64plus.desktop "$(DESTDIR)$(APPSDIR)"
	$(INSTALL) -d "$(DESTDIR)$(ICONSDIR)/48x48/apps"
	$(INSTALL) -m 0644 $(SRCDIR)/../data/icons/48x48/apps/mupen64plus.png "$(DESTDIR)$(ICONSDIR)/48x48/apps"
	$(INSTALL) -d "$(DESTDIR)$(ICONSDIR)/scalable/apps"
	$(INSTALL) -m 0644 $(SRCDIR)/../data/icons/scalable/apps/mupen64plus.svg "$(DESTDIR)$(ICONSDIR)/scalable/apps"

uninstall:
	$(RM) "$(DESTDIR)$(BINDIR)/$(TARGET)" "$(DESTDIR)$(MANDIR)/man6/mupen64plus.6"
	$(RM) "$(DESTDIR)$(APPSDIR)/mupen64plus.desktop"
	$(RM) "$(DESTDIR)$(ICONSDIR)/48x48/apps/mupen64plus.png"
	$(RM) "$(DESTDIR)$(ICONSDIR)/scalable/apps/mupen64plus.svg"

# standard build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: all clean install uninstall help
