# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#   config/scotch.sh
#
# Description
#   Setup file for scotch include/libraries.
#   Sourced during wmake process only.

_extract_value() {
	if [ "$#" -lt 2 ]
	then
		echo ""
		return
	fi

	local header="$1"
	local variable="$2"

	echo $(cpp -dM $header | grep $2 | cut -d" " -f3)
}

get_scotch_version () {
	local version=
	local header=

	[ -f "/usr/include/scotch.h" ] \
		&& header="/usr/include/scotch.h"

	[ -f "/usr/local/include/scotch.h" ] \
		&& header="/usr/local/include/scotch.h"

	if [ -f $header ]
	then
		local ver=$(_extract_value $header SCOTCH_VERSION)
		local rel=$(_extract_value $header SCOTCH_RELEASE)
		local ptc=$(_extract_value $header SCOTCH_PATCHLEVEL)

		version="$ver.$rel.$ptc"
	fi

	echo $version
}

get_scotch_arch_path () {
	[ -f "/usr/include/scotch.h" ] && echo "/usr"
	[ -f "/usr/include/scotch/scotch.h" ] && echo "/usr"
	[ -f "/usr/local/include/scotch.h" ] && echo "/usr/local"
}

if [ $(uname -s) = "Darwin" ]
then
	_brew_prefix="$(brew --prefix)"
	if [ "$WM_LABEL_SIZE" -eq "32" ]
	then
		export SCOTCH_VERSION="scotch-$(basename $(readlink "$_brew_prefix/opt/scotch"))"
		export SCOTCH_ARCH_PATH="$_brew_prefix/opt/scotch"
	else
		export SCOTCH_VERSION="scotch-$(basename $(readlink "$_brew_prefix/opt/scotch64"))"
		export SCOTCH_ARCH_PATH="$_brew_prefix/opt/scotch64"
	fi
	unset _brew_prefix
else
	export SCOTCH_VERSION=scotch-$(get_scotch_version)
	export SCOTCH_ARCH_PATH=$(get_scotch_arch_path)
fi

# vim: set ft=sh noet sw=2 ts=2 sts=2:
