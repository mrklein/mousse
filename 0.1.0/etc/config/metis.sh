# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
# File
#     config/metis.sh
#
# Description
#     Setup file for metis include/libraries.
#     Sourced during wmake process only.

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
	export METIS_VERSION=metis-5.1.0
	export METIS_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$METIS_VERSION
fi

# -----------------------------------------------------------------------------
