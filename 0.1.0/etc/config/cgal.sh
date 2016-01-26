# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#   config/cgal.sh
# Description
#   Setup file for CGAL (& boost) include/libraries.
#   Sourced from <VERSION>/etc/bashrc

get_boost_version() {
	local version_header=
	local version=

	# System-wide installation
	[ -f "/usr/include/boost/version.hpp" ] \
		&& version_header="/usr/include/boost/version.hpp"

	# Another variant of system-wide installtion
	[ -f "/usr/local/include/boost/version.hpp" ] \
		&& version_header="/usr/local/include/boost/version.hpp"

	if [ -f "$version_header" ]
	then
		version=$(cpp -undef -dM $version_header \
		| grep BOOST_LIB_VERSION \
		| cut -d" " -f 3 \
		| tr -d '"' \
		| tr _ .)
	fi

	echo $version
}

set_boost_arch_path () {
	# System-wide installation
	[ -f "/usr/include/boost/version.hpp" ] && BOOST_ARCH_PATH="/usr"

	# Another variant of system-wide installtion
	[ -f "/usr/local/include/boost/version.hpp" ] \
		&& BOOST_ARCH_PATH="/usr/local"
}

get_cgal_version() {
	local version_header=
	local version=

	# System-wide installation
	[ -f "/usr/include/CGAL/version.h" ] \
		&& version_header="/usr/include/CGAL/version.h"

	# Another variant of system-wide installtion
	[ -f "/usr/local/include/CGAL/version.h" ] \
		&& version_header="/usr/local/include/CGAL/version.h"

	if [ -f "$version_header" ]
	then
		version=$(cpp -undef -dM $version_header \
		| grep ' CGAL_VERSION ' \
		| cut -d" " -f 3)
	fi

	echo $version
}

set_cgal_arch_path () {
	# System-wide installation
	[ -f "/usr/include/CGAL/version.h" ] \
		&& CGAL_ARCH_PATH="/usr"

	# Another variant of system-wide installtion
	[ -f "/usr/local/include/CGAL/version.h" ] \
		&& CGAL_ARCH_PATH="/usr/local"
}

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
	BOOST_ARCH_PATH=
	CGAL_ARCH_PATH=

	boost_version=boost-$(get_boost_version)
	cgal_version=CGAL-$(get_cgal_version)

	set_boost_arch_path
	set_cgal_arch_path

	export BOOST_ARCH_PATH
	export CGAL_ARCH_PATH
fi

if [ "$FOAM_VERBOSE" -a "$PS1" ]
then
	echo "Using CGAL and boost" 1>&2
	echo "    $cgal_version at $CGAL_ARCH_PATH" 1>&2
	echo "    $boost_version at $BOOST_ARCH_PATH" 1>&2
fi

[ -d "$CGAL_ARCH_PATH" ] && {
[ -d "$CGAL_ARCH_PATH/lib" ] \
	&& _mousse_add_lib "$CGAL_ARCH_PATH/lib"
[ -d "$CGAL_ARCH_PATH/lib64" ] \
	&& _mousse_add_lib "$CGAL_ARCH_PATH/lib64"
}

[ -d "$BOOST_ARCH_PATH" ] && {
[ -d "$BOOST_ARCH_PATH/lib" ] \
	&& _mousse_add_lib "$BOOST_ARCH_PATH/lib"
[ -d "$BOOST_ARCH_PATH/lib64" ] \
	&& _mousse_add_lib "$BOOST_ARCH_PATH/lib64"
}

unset boost_version cgal_version get_boost_version get_cgal_version

# vim: set ft=sh noet sw=2 ts=2 sts=2:
