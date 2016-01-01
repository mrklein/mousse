# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
#
# File
#     etc/config/unset.sh
#
# Description
#     Clear as many mousse environment settings as possible

mousse_clean=$WM_PROJECT_DIR/bin/mousse-clean-path
[ -f "$mousse_clean" -a -x "$mousse_clean" ] || unset mousse_clean

mousse_old_dirs="$WM_PROJECT_INST_DIR $WM_PROJECT_SITE"
if [ -n "$WM_PROJECT" ]
then
    mousse_old_dirs="$mousse_old_dirs $HOME/$WM_PROJECT/$USER_SITE"
fi

# unset WM_* environment variables
unset WM_ARCH
unset WM_ARCH_OPTION
unset WM_CC
unset WM_CFLAGS
unset WM_COMPILER
unset WM_COMPILER_LIB_ARCH
unset WM_COMPILE_OPTION
unset WM_CXX
unset WM_CXXFLAGS
unset WM_DIR
unset WM_HOSTS
unset WM_LDFLAGS
unset WM_LINK_LANGUAGE
unset WM_MPLIB
unset WM_NCOMPPROCS
unset WM_OPTIONS
unset WM_OSTYPE
unset WM_PRECISION_OPTION
unset WM_PROJECT
unset WM_PROJECT_DIR
unset WM_PROJECT_INST_DIR
unset WM_PROJECT_SITE
unset WM_PROJECT_USER_DIR
unset WM_PROJECT_VERSION
unset WM_SCHEDULER
unset WM_SILENT_RULES

# unset MOUSSE_* environment variables
unset MOUSSE_APPBIN
unset MOUSSE_APP
unset MOUSSE_EXT_LIBBIN
unset MOUSSE_CODE_TEMPLATES
unset MOUSSE_INST_DIR
unset MOUSSE_JOB_DIR
unset MOUSSE_LIBBIN
unset MOUSSE_MPI
unset MOUSSE_RUN
unset MOUSSE_SETTINGS
unset MOUSSE_SIGFPE
unset MOUSSE_SIGNAN
unset MOUSSE_SITE_APPBIN
unset MOUSSE_SITE_LIBBIN
unset MOUSSE_SOLVERS
unset MOUSSE_SRC
unset MOUSSE_TUTORIALS
unset MOUSSE_USER_APPBIN
unset MOUSSE_USER_LIBBIN
unset MOUSSE_UTILITIES


# unset MPI-related environment variables
unset MPI_ARCH_PATH
unset MPI_BUFFER_SIZE
unset OPAL_PREFIX

# cleanup environment
# PATH, LD_LIBRARY_PATH, MANPATH
if [ -n "$mousse_clean" ]
then
    cleaned=`$mousse_clean "$PATH" "$mousse_old_dirs"` && PATH="$cleaned"
    if [ "$(uname -s)" = "Darwin" ]
    then
        cleaned=`$mousse_clean "$DYLD_LIBRARY_PATH" "$mousse_old_dirs"` && DYLD_LIBRARY_PATH="$cleaned"
    else
        cleaned=`$mousse_clean "$LD_LIBRARY_PATH" "$mousse_old_dirs"` && LD_LIBRARY_PATH="$cleaned"
    fi
    cleaned=`$mousse_clean "$MANPATH" "$mousse_old_dirs"` && MANPATH="$cleaned"
fi


if [ "$(uname -s)" = "Darwin" ]
then
    [ -n "$DYLD_LIBRARY_PATH" ] || unset DYLD_LIBRARY_PATH
else
    [ -n "$LD_LIBRARY_PATH" ] || unset LD_LIBRARY_PATH
fi
[ -n "$MANPATH" ] || unset MANPATH
[ -n "$LD_PRELOAD" ] || unset LD_PRELOAD

unset cleaned mousse_clean mousse_old_dirs

# cleanup aliases

unalias wmSET
unalias wm64
unalias wm32
unalias wmSP
unalias wmDP

unalias wmUNSET

unset foamPV

unalias src
unalias lib
unalias run
unalias mousse
unalias moussesrc
unalias moussefv
unalias app
unalias util
unalias sol
unalias tut

unalias mousse_apps
unalias mousse_sol
unalias mousse_tuts
unalias mousse_utils
unalias mousse_site
