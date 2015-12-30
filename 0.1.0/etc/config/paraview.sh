# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     config/paraview.sh
#
# Description
#     Setup file for paraview-[3-4].x on Linux
#     Sourced from <VERSION>/etc/bashrc or from moussePV alias

# clean the PATH
cleaned=`$WM_PROJECT_DIR/bin/mousse-clean-path "$PATH" "$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/cmake- $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/paraview-"` && PATH="$cleaned"

# determine the cmake to be used
unset CMAKE_HOME
for cmake in cmake-3.2.1 cmake-2.8.12.1 cmake-2.8.8 cmake-2.8.4 cmake-2.8.3 cmake-2.8.1
do
    cmake=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$cmake
    if [ -r $cmake ]
    then
        export CMAKE_HOME=$cmake
        export PATH=$CMAKE_HOME/bin:$PATH
        break
    fi
done


#- ParaView version, automatically determine major version
#export ParaView_VERSION=3.12.0
#export ParaView_VERSION=4.0.1
#export ParaView_VERSION=4.1.0
#export ParaView_VERSION=4.3.1
export ParaView_VERSION=4.4.0
export ParaView_MAJOR=detect


# Evaluate command-line parameters for ParaView
_mousse_paraview_eval()
{
    while [ $# -gt 0 ]
    do
        case "$1" in
        ParaView*=*)
            # name=value  -> export name=value
            eval "export $1"
            ;;
        esac
        shift
    done
}

# Evaluate command-line parameters
_mousse_paraview_eval $@


# set MAJOR version to correspond to VERSION
# ParaView_MAJOR is "<digits>.<digits>" from ParaView_VERSION
case "$ParaView_VERSION" in
"$ParaView_MAJOR".* )
    # version and major appear to correspond
    ;;

[0-9]*)
    # extract major from the version
    ParaView_MAJOR=`echo $ParaView_VERSION | sed -e 's/^\([0-9][0-9]*\.[0-9][0-9]*\).*$/\1/'`
    ;;
esac
export ParaView_VERSION ParaView_MAJOR

paraview_inst_dir=$WM_THIRD_PARTY_DIR/ParaView-$ParaView_VERSION
paraview_arch_name=ParaView-$ParaView_VERSION

export ParaView_DIR=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$paraviewArchName

# set paths if binaries or source are present
if [ -r $ParaView_DIR -o -r $paraview_inst_dir ]
then
    export ParaView_INCLUDE_DIR=$ParaView_DIR/include/paraview-$ParaView_MAJOR
    if [ ! -d $ParaView_INCLUDE_DIR -a -d $ParaView_DIR/include/paraview ]
    then
        export ParaView_INCLUDE_DIR=$ParaView_DIR/include/paraview
    fi

    ParaView_LIB_DIR=$ParaView_DIR/lib/paraview-$ParaView_MAJOR
    if [ ! -d $ParaView_LIB_DIR -a -d $ParaView_DIR/lib/paraview ]
    then
        ParaView_LIB_DIR=$ParaView_DIR/lib/paraview
    fi

    export PATH=$ParaView_DIR/bin:$PATH
    export LD_LIBRARY_PATH=$ParaView_LIB_DIR:$LD_LIBRARY_PATH
    export PV_PLUGIN_PATH=$FOAM_LIBBIN/paraview-$ParaView_MAJOR

    if [ "$MOUSSE_VERBOSE" -a "$PS1" ]
    then
        echo "Using paraview"
        echo "    ParaView_DIR         : $ParaView_DIR"
        echo "    ParaView_LIB_DIR     : $ParaView_LIB_DIR"
        echo "    ParaView_INCLUDE_DIR : $ParaView_INCLUDE_DIR"
        echo "    PV_PLUGIN_PATH       : $PV_PLUGIN_PATH"
    fi

    # add in python libraries if required
    paraviewPython=$ParaView_DIR/Utilities/VTKPythonWrapping
    if [ -r $paraview_python ]
    then
        if [ "$PYTHONPATH" ]
        then
            export PYTHONPATH=$PYTHONPATH:$paraview_python:$ParaView_LIB_DIR
        else
            export PYTHONPATH=$paraview_python:$ParaView_LIB_DIR
        fi
    fi
else
    unset PV_PLUGIN_PATH
fi

unset _foam_paraview_eval
unset cleaned cmake paraview_inst_dir paraview_python

# vim: set ft=sh noet sw=4 ts=4 sts=4:
