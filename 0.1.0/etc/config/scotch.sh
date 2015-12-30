# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     config/scotch.sh
#
# Description
#     Setup file for scotch include/libraries.
#     Sourced during wmake process only.

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
	export SCOTCH_VERSION=scotch-6.0.3
	export SCOTCH_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER$WM_LABEL_OPTION/$SCOTCH_VERSION
fi

# vim: set ft=sh noet sw=4 ts=4 sts=4:
