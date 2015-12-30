# mousse: CFD toolbox
# Copyright (C) 2011-2014 OpenFOAM Foundation
# Copyright (C) 2016 mousse project

if [ -d "/Applications/paraview.app" ]; then

	[ "$FOAM_VERBOSE" -a "$PS1" ] && echo "Using /Applications/paraview.app"

	paraview () {
		"/Applications/paraview.app/Contents/MacOS/paraview" "$@"
	}

	export -f paraview
else
	echo
	echo "Please download binary Paraview release from"
	echo "http://paraview.org and install it in /Applications"
	echo "folder."
	echo
fi
