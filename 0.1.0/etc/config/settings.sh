# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
#
# File
#     etc/config/settings.sh
#
# Description
#     Startup file for mousse
#     Sourced from <VERSION>/etc/bashrc

# Increase maximum number of open files if it is below limit
_increase_max_open_files()
{
    # Default value
    local lim=4096

    # Reset if parameter passed
    [ -n "$1" ] && lim="$1"

    # Silencing the errors
    [ "$(ulimit -n)" -lt "$lim" ] && ulimit -n "$lim" > /dev/null 2>&1
}

# Check if mousse is built on OS X
_mousse_on_osx()
{
    if [ "$(uname -s)" = "Darwin" ]
    then
        return 0
    else
        return 1
    fi
}

# Prefix to PATH
_mousse_add_path()
{
    while [ $# -ge 1 ]
    do
        export PATH=$1:$PATH
        shift
    done
}

# Prefix to LD_LIBRARY_PATH (or DYLD_LIBRARY_PATH on OS X)
_mousse_add_lib()
{
    while [ $# -ge 1 ]
    do
        if _mousse_on_osx
        then
            export DYLD_LIBRARY_PATH=$1:$DYLD_LIBRARY_PATH
        else
            export LD_LIBRARY_PATH=$1:$LD_LIBRARY_PATH
        fi
        shift
    done
}

# Prefix to MANPATH
_mousse_add_man()
{
    while [ $# -ge 1 ]
    do
        export MANPATH=$1:$MANPATH
        shift
    done
}

# Set environment variables according to system type
export WM_ARCH=$(uname -s | tr '[:upper:]' '[:lower:]')

case "$WM_ARCH" in
linux)
    WM_ARCH=linux

    # compiler specifics
    case $(uname -m) in
        i686)
            export WM_ARCH_OPTION=32
        ;;

    x86_64)
        case "$WM_ARCH_OPTION" in
        32)
            export WM_COMPILER_ARCH=64
            export WM_CC='gcc'
            export WM_CXX='g++'
            export WM_CFLAGS='-m32 -fPIC'
            export WM_CXXFLAGS='-m32 -fPIC'
            export WM_LDFLAGS='-m32'
            ;;
        64)
            WM_ARCH=linux64
            export WM_COMPILER_LIB_ARCH=64
            export WM_CC='gcc'
            export WM_CXX='g++'
            export WM_CFLAGS='-m64 -fPIC'
            export WM_CXXFLAGS='-m64 -fPIC'
            export WM_LDFLAGS='-m64'
            ;;
        *)
            echo "Unknown WM_ARCH_OPTION '$WM_ARCH_OPTION', should be 32 or 64"\
                 1>&2
            ;;
        esac
        ;;

    ia64)
        WM_ARCH=linux_ia64
        export WM_COMPILER=I64
        ;;

    armv7l)
        WM_ARCH=linux_arm7
        export WM_COMPILER_LIB_ARCH=32
        export WM_CC='gcc'
        export WM_CXX='g++'
        export WM_CFLAGS='-fPIC'
        export WM_CXXFLAGS='-fPIC'
        export WM_LDFLAGS=
        ;;

    ppc64)
        WM_ARCH=linux_ppc64
        export WM_COMPILER_LIB_ARCH=64
        export WM_CC='gcc'
        export WM_CXX='g++'
        export WM_CFLAGS='-m64 -fPIC'
        export WM_CXXFLAGS='-m64 -fPIC'
        export WM_LDFLAGS='-m64'
        ;;

    ppc64le)
        WM_ARCH=linux_ppc64le
        export WM_COMPILER_LIB_ARCH=64
        export WM_CC='gcc'
        export WM_CXX='g++'
        export WM_CFLAGS='-m64 -fPIC'
        export WM_CXXFLAGS='-m64 -fPIC'
        export WM_LDFLAGS='-m64'
        ;;

    *)
        echo Unknown processor type `uname -m` for Linux 1>&2
        ;;
    esac
    ;;

darwin)
    WM_ARCH=darwin64
    export WM_COMPILER_LIB_ARCH=64
    export WM_CC='clang'
    export WM_CXX='clang++'
    ;;

*)    # an unsupported operating system
    /bin/cat <<USAGE 1>&2

    Your "$WM_ARCH" operating system is not supported by this release
    of mousse.

USAGE
    ;;
esac


#------------------------------------------------------------------------------

# Location of the jobControl directory
export MOUSSE_JOB_DIR=$WM_PROJECT_INST_DIR/job_control

# wmake configuration
export WM_DIR=$WM_PROJECT_DIR/wmake
export WM_LINK_LANGUAGE=c++
export WM_LABEL_OPTION=int$WM_LABEL_SIZE
export WM_OPTIONS=${WM_ARCH}_${WM_COMPILER}_${WM_PRECISION_OPTION}_${WM_LABEL_OPTION}_${WM_COMPILE_OPTION}

# Base executables/libraries
export MOUSSE_APPBIN=$WM_PROJECT_DIR/platforms/$WM_OPTIONS/bin
export MOUSSE_LIBBIN=$WM_PROJECT_DIR/platforms/$WM_OPTIONS/lib

# Site-specific directory
site_dir="${WM_PROJECT_SITE:-$WM_PROJECT_INST_DIR/site}"

# Shared site executables/libraries
# Similar naming convention as ~OpenFOAM expansion
export MOUSSE_SITE_APPBIN=$site_dir/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/bin
export MOUSSE_SITE_LIBBIN=$site_dir/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/lib

# User executables/libraries
export MOUSSE_USER_APPBIN=$WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/bin
export MOUSSE_USER_LIBBIN=$WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/lib

# DynamicCode templates
# - default location is the "~mousse/codeTemplates/dynamicCode" expansion
# export MOUSSE_CODE_TEMPLATES=$WM_PROJECT_DIR/etc/codeTemplates/dynamicCode

# Convenience
export MOUSSE_ETC=$WM_PROJECT_DIR/etc
export MOUSSE_APP=$WM_PROJECT_DIR/applications
export MOUSSE_SRC=$WM_PROJECT_DIR/src
export MOUSSE_TUTORIALS=$WM_PROJECT_DIR/tutorials
export MOUSSE_UTILITIES=$FOAM_APP/utilities
export MOUSSE_SOLVERS=$FOAM_APP/solvers
export MOUSSE_RUN=$WM_PROJECT_USER_DIR/run

# Add wmake to the path - not required for runtime-only environment
[ -d "$WM_DIR" ] && PATH=$WM_DIR:$PATH
# Add mousse scripts to the path
export PATH=$WM_PROJECT_DIR/bin:$PATH

# add site-specific scripts to path - only if they exist
if [ -d "$site_dir/bin" ]                        # generic
then
    _mousse_add_path "$site_dir/bin"
fi
if [ -d "$site_dir/$WM_PROJECT_VERSION/bin" ]    # version-specific
then
    _mousse_add_path "$site_dir/$WM_PROJECT_VERSION/bin"
fi
unset site_dir

_mousse_add_path $MOUSSE_USER_APPBIN:$MOUSSE_SITE_APPBIN:$MOUSSE_APPBIN
# Make sure to pick up dummy versions of external libraries last
if [ -n "$MOUSSE_EXT_LIBBIN" ]
then
    _mousse_add_lib  $MOUSSE_USER_LIBBIN:$MOUSSE_SITE_LIBBIN:$MOUSSE_LIBBIN:$MOUSSE_EXT_LIBBIN:$FOAM_LIBBIN/dummy
else
    _mousse_add_lib  $MOUSSE_USER_LIBBIN:$MOUSSE_SITE_LIBBIN:$MOUSSE_LIBBIN:$MOUSSE_LIBBIN/dummy
fi

# Compiler settings
unset gcc_version gmp_version mpfr_version mpc_version
unset MPFR_ARCH_PATH GMP_ARCH_PATH

# Location of compiler installation
[ -z "$mousse_compiler" ] && mousse_compiler=system

# Communications library
unset MPI_ARCH_PATH MPI_HOME FOAM_MPI_LIBBIN

# Use the system installed openmpi, get library directory via mpicc
export MOUSSE_MPI=openmpi-system

lib_dir=$(mpicc --showme:link | sed -e 's/.*-L\([^ ]*\).*/\1/')

# Bit of a hack: strip off 'lib' and hope this is the path to openmpi
# include files and libraries.
export MPI_ARCH_PATH="${lib_dir%/*}"

_mousse_add_lib     $lib_dir
unset lib_dir

# Add (non-dummy) MPI implementation
# Dummy MPI already added to LD_LIBRARY_PATH and has no external libraries
if [ "$MOUSSE_MPI" != dummy ]
then
    if _mousse_on_osx
    then
        _mousse_add_lib $MOUSSE_LIBBIN/$MOUSSE_MPI
    else
        _mousse_add_lib $MOUSSE_LIBBIN/$MOUSSE_MPI:$MOUSSE_EXT_LIBBIN/$MOUSSE_MPI
    fi
fi

# Set the minimum MPI buffer size (used by all platforms except SGI MPI)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
: ${min_buffer_size:=20000000}

if [ "${MPI_BUFFER_SIZE:=$min_buffer_size}" -lt $min_buffer_size ]
then
    MPI_BUFFER_SIZE=$min_buffer_size
fi
export MPI_BUFFER_SIZE

_mousse_on_osx && _increase_max_open_files 4096

# Cleanup environment:
# ~~~~~~~~~~~~~~~~~~~~
#keep _mousse_add_path _mousse_add_lib _mousse_add_man _mousse_on_osx
unset mousse_compiler min_buffer_size _increase_max_open_files

# vim: set ft=sh noet sw=4 ts=4 sts=4:
