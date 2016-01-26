# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project
#
# File
#     etc/config/aliases.sh
#
# Description
#     Aliases for working with mousse
#     Sourced from <VERSION>/etc/bashrc and/or ~/.bashrc

# Change compiled version aliases
alias wmSET='. $WM_PROJECT_DIR/etc/bashrc'
alias wm64='wmSET WM_ARCH_OPTION=64'
alias wm32='wmSET WM_ARCH_OPTION=32'
alias wmSP='wmSET WM_PRECISION_OPTION=SP'
alias wmDP='wmSET WM_PRECISION_OPTION=DP'

# refresh the environment
alias wmREFRESH='wmSET $MOUSSE_SETTINGS'

# clear env
alias wmUNSET='. $WM_PROJECT_DIR/etc/config/unset.sh'

# Change directory aliases
alias src='cd $MOUSSE_SRC'
alias lib='cd $MOUSSE_LIBBIN'
alias run='cd $MOUSSE_RUN'
alias mousse='cd $WM_PROJECT_DIR'
alias moussesrc='cd $MOUSSE_SRC/$WM_PROJECT'
alias moussefv='cd $MOUSSE_SRC/finite_volume'
alias app='cd $MOUSSE_APP'
alias util='cd $MOUSSE_UTILITIES'
alias sol='cd $MOUSSE_SOLVERS'
alias tut='cd $MOUSSE_TUTORIALS'

alias mousse_apps='cd $MOUSSE_APP'
alias mousse_sol='cd $MOUSSE_SOLVERS'
alias mousse_tuts='cd $MOUSSE_TUTORIALS'
alias mousse_utils='cd $MOUSSE_UTILITIES'

if [ -n "$WM_PROJECT_SITE" ]
then
	alias mousse_site='cd $WM_PROJECT_SITE'
else
	alias mousse_site='cd $WM_PROJECT_INST_DIR/site'
fi

# vim: set ft=sh noet sw=2 ts=2 sts=2:
