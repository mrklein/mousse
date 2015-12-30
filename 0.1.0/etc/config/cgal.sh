# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     config/cgal.sh
# Description
#     Setup file for CGAL (& boost) include/libraries.
#     Sourced from <VERSION>/etc/bashrc

if [ "$(uname -s)" = "Darwin" ]
then
    _brew_prefix="$(brew --prefix)"

	boost_version="boost-$(basename $(readlink "$_brew_prefix/opt/boost"))"
	cgal_version="CGAL-$(basename $(readlink "$_brew_prefix/opt/cgal"))"

	export BOOST_ARCH_PATH="$_brew_prefix/opt/boost"
	export CGAL_ARCH_PATH="$_brew_prefix/opt/cgal"
	export GMP_ARCH_PATH="$_brew_prefix/opt/gmp"
	export MPFR_ARCH_PATH="$_brew_prefix/opt/mpfr"

    unset _brew_prefix
else
	boost_version=boost-system
	cgal_version=CGAL-4.7

	export BOOST_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$boost_version
	export CGAL_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$cgal_version
fi

if [ "$FOAM_VERBOSE" -a "$PS1" ]
then
    echo "Using CGAL and boost" 1>&2
    echo "    $cgal_version at $CGAL_ARCH_PATH" 1>&2
    echo "    $boost_version at $BOOST_ARCH_PATH" 1>&2
fi

if [ -d "$CGAL_ARCH_PATH" ]
then
    _mousse_add_lib "$CGAL_ARCH_PATH/lib"
fi

if [ -d "$BOOST_ARCH_PATH" ]
then
    _mousse_add_lib "$BOOST_ARCH_PATH/lib"
fi

unset boost_version cgal_version

# vim: set ft=sh noet sw=4 ts=4 sts=4:
