# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# 
# File
#     makefile
# Description
#     A generic Makefile, used by wmake

SHELL      = /bin/sh

# Unset suffices list (suffix rules are not used)
.SUFFIXES:

# Set the directory containing the wmake scripts
WM_SCRIPTS   = $(WM_DIR)/scripts

# Declare default paths
LIB_SRC            = $(WM_PROJECT_DIR)/src
LIB_PLATFORMS      = $(WM_PROJECT_DIR)/platforms/$(WM_OPTIONS)/lib
OBJECTS_DIR        = $(MAKE_DIR)/$(WM_OPTIONS)

SYS_INC            =
SYS_LIBS           =

PROJECT_INC        = -I$(LIB_SRC)/core/_lninclude \
		     -I$(LIB_SRC)/os_specific/$(WM_OSTYPE)/_lninclude

PROJECT_LIBS       = -l$(WM_PROJECT)

EXE_INC            =
EXE_LIBS           =

LIB_LIBS           =

# Declare default name of libraries and executables
# Library
LIB             = libNULL

# Shared library extension
ifeq ($(shell uname -s),Darwin)
SO              = dylib
else
SO              = so
endif

# Project executable
EXE             = $(WM_PROJECT).out

# Standalone executable
SEXE            = a.out

# Set compilation and dependency building rules
GENERAL_RULES = $(WM_DIR)/rules/general
RULES         = $(WM_DIR)/rules/$(WM_ARCH)_$(WM_COMPILER)
WMAKE_BIN     = $(WM_DIR)/platforms/$(WM_ARCH)_$(WM_COMPILER)

include $(GENERAL_RULES)/general
include $(RULES)/general
include $(RULES)/$(WM_LINK_LANGUAGE)
include $(GENERAL_RULES)/transform

# Include PROJECT directory tree file and
# source, object and dependency list files.
# These are constructed by makefile_files
include $(OBJECTS_DIR)/options
include $(OBJECTS_DIR)/variables
include $(OBJECTS_DIR)/source_files
DEPENDENCIES=$(SOURCE:%=$(OBJECTS_DIR)/%.dep)
BASENAMES=$(basename $(SOURCE))
OBJECTS=$(BASENAMES:%=$(OBJECTS_DIR)/%.o)

# Set header file include paths
LIB_HEADER_DIRS = \
        $(EXE_INC) \
        -I_lninclude \
        -I. \
        $(PROJECT_INC) \
        $(GINC) \
        $(SYS_INC)

# Define link statements for executables
$(EXE): $(OBJECTS)
	@$(WM_SCRIPTS)/make-target-dir $(EXE)
ifdef WM_SILENT_RULES
	@echo "  [LD] $(notdir $(EXE))"
	@$(LINKEXE) $(OBJECTS) -L$(LIB_PLATFORMS) \
		$(EXE_LIBS) $(PROJECT_LIBS) $(SYS_LIBS) \
		$(LINK_LIBS) $(GLIBS) -o $(EXE)
else
	$(LINKEXE) $(OBJECTS) -L$(LIB_PLATFORMS) \
		$(EXE_LIBS) $(PROJECT_LIBS) $(SYS_LIBS) \
		$(LINK_LIBS) $(GLIBS) -o $(EXE)
endif

exe:    $(SEXE)
ifdef WM_SILENT_RULES
	@echo "-> $(notdir SEXE)"
else
	@echo \'$(SEXE)\' is up to date.
endif

$(SEXE):$(OBJECTS)
	@$(WM_SCRIPTS)/make-target-dir $(SEXE)
ifdef WM_SILENT_RULES
	@echo "  [LD] $(notdir $(EXE))"
	@$(LINKEXE) $(OBJECTS) $(EXE_LIBS) \
		$(SYS_LIBS) $(LINK_LIBS) $(GLIBS) -o $(SEXE)
else
	$(LINKEXE) $(OBJECTS) $(EXE_LIBS) \
		$(SYS_LIBS) $(LINK_LIBS) $(GLIBS) -o $(SEXE)

endif

# Define link statements for libraries
objects: $(OBJECTS)

libso:  $(LIB).$(SO)
ifdef WM_SILENT_RULES
	@echo "-> $(notdir $(LIB)).$(SO)"
else
	@echo \'$(LIB).$(SO)\' is up to date.
endif

$(LIB).$(SO): $(OBJECTS)
	@$(WM_SCRIPTS)/make-target-dir $(LIB)
ifdef WM_SILENT_RULES
	@echo "  [LD] $(notdir $(LIB)).$(SO)"
	@$(LINKLIBSO) $(OBJECTS) -L$(LIB_PLATFORMS) \
		$(LIB_LIBS) $(GLIB_LIBS) -o $(LIB).$(SO)
else
	$(LINKLIBSO) $(OBJECTS) -L$(LIB_PLATFORMS) \
		$(LIB_LIBS) $(GLIB_LIBS) -o $(LIB).$(SO)
endif

lib:    $(LIB).a
ifdef WM_SILENT_RULES
	@echo "-> $(notdir $(LIB)).a"
else
	@echo \'$(LIB).a\' is up to date.
endif

$(LIB).a: $(OBJECTS)
	@$(WM_SCRIPTS)/make-target-dir $(LIB)
	@rm -f $(LIB).a
ifdef WM_SILENT_RULES
	@echo "  [AR] $(LIB)"
	@$(AR) $(ARFLAGS) $(notdir $(LIB)).a $(OBJECTS)
else
	$(AR) $(ARFLAGS) $(LIB).a $(OBJECTS)
endif
ifdef WM_SILENT_RULES
	@echo "  [RANLIB] $(notdir $(LIB))"
	@$(RANLIB) $(LIB).a
else
	$(RANLIB) $(LIB).a
endif

libo:   $(LIB).o
ifdef WM_SILENT_RULES
	@echo "-> $(notdir $(LIB)).o"
else
	@echo \'$(LIB).o\' is up to date.
endif

$(LIB).o: $(OBJECTS)
	@$(WM_SCRIPTS)/make-target-dir $(LIB)
	@rm -f $(LIB).o
ifdef WM_SILENT_RULES
	@echo "  [LD] $(notdir $(LIB)).o"
	@$(LD) -r -o $(LIB).o $(OBJECTS)
else
	$(LD) -r -o $(LIB).o $(OBJECTS)
endif

# Set rule to create the include directory for libraries
_lninclude: $(MAKE_DIR)/files $(MAKE_DIR)/options
	@rm -rf _lninclude ; wmake-lninclude .

# Declare all qbject files depend on $(OBJECTS_DIR)/options
$(OBJECTS) : $(OBJECTS_DIR)/options
$(foreach S,$(SOURCE),$(eval $(OBJECTS_DIR)/$(basename $S).o : $(OBJECTS_DIR)/$S.dep))

# Set depedency rule and include dependency lists
.PHONY: dep
dep: $(DEPENDENCIES)

ifeq ($(findstring _lninclude,$(MAKECMDGOALS)),)
-include $(DEPENDENCIES)
endif

# vim: set ft=make:
