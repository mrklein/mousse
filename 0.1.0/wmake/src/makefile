# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     makefile
# Description
#     Makefile used to compile wmake utilities.

# The Makefile use a POSIX shell
SHELL      = /bin/sh

# Unset suffices list (suffix rules are not used)
.SUFFIXES:

# set compilation and dependency building rules
GENERAL_RULES = $(WM_DIR)/rules/general
RULES         = $(WM_DIR)/rules/$(WM_ARCH)_$(WM_COMPILER)
WMAKE_BIN     = $(WM_DIR)/platforms/$(WM_ARCH)_$(WM_COMPILER)

include $(RULES)/general
include $(RULES)/$(WM_LINK_LANGUAGE)

cFLAGS += -Wno-unneeded-internal-declaration -Wno-unused-function

# targets
all:  $(WMAKE_BIN)/dir-to-string $(WMAKE_BIN)/wmkdep

clean:
	@$(RM) $(WMAKE_BIN)/* 2>/dev/null

$(WMAKE_BIN)/dir-to-string: dir_to_string.c
	@mkdir -p $(WMAKE_BIN)
	$(cc) $(cFLAGS) dir_to_string.c -o $(WMAKE_BIN)/dir-to-string

$(WMAKE_BIN)/wmkdep: wmkdep.l
	@mkdir -p $(WMAKE_BIN)
	$(LEX) wmkdep.l
	$(cc) $(cFLAGS) lex.yy.c -o $(WMAKE_BIN)/wmkdep
	@$(RM) lex.yy.c 2>/dev/null

