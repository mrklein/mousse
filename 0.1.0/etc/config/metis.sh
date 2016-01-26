# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     config/metis.sh
#
# Description
#     Setup file for metis include/libraries.
#     Sourced during wmake process only.

extract_value () {
	if [ "$#" -lt 2 ]
	then
		echo ""
		return
	fi

	local header="$1"
	local variable="$2"

	echo $(cpp -dM $header | grep $variable | cut -d " " -f 3)
}

get_metis_version () {
	local version=
	local version_header=

	# System-wide installation
	[ -f "/usr/include/metis.h" ] \
		&& version_header="/usr/include/metis.h"

	# Another variant of system-wide installation
	[ -f "/usr/local/include/metis.h" ] \
		&& version_header="/usr/local/include/metis.h"

	if [ -f "$version_header" ]
	then
		local major=$(extract_value $version_header METIS_VER_MAJOR)
		local minor=$(extract_value $version_header METIS_VER_MINOR)
		local subminor=$(extract_value $version_header METIS_VER_SUBMINOR)

		version="${major}.${minor}.${subminor}"
	fi

	echo $version
}

get_metis_arch_path () {
	# System-wide installation
	[ -f "/usr/include/metis.h" ] && echo "/usr"
	# Another variant of system-wide installation
	[ -f "/usr/local/include/metis.h" ] && echo "/usr/local"
}

if [ "$(uname -s)" = "Darwin" ]
then
	_brew_prefix=$(brew --prefix)
	if [ "$WM_LABEL_SIZE" -eq 32 ]
	then
		export METIS_VERSION="metis-$(basename $(readlink "$_brew_prefix/opt/metis"))"
		export METIS_ARCH_PATH="$_brew_prefix/opt/metis"
	else
		export METIS_VERSION="metis-$(basename $(readlink "$_brew_prefix/opt/metis64"))"
		export METIS_ARCH_PATH="$_brew_prefix/opt/metis64"
	fi
	unset _brew_prefix
else
	METIS_ARCH_PATH=
	export METIS_VERSION=metis-$(get_metis_version)
	export METIS_ARCH_PATH=$(get_metis_arch_path)
fi

unset extract_value get_metis_version get_metis_arch_path

# vim: set ft=sh noet sw=2 ts=2 sts=2:
