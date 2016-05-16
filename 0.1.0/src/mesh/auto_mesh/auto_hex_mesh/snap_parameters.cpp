// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "snap_parameters.hpp"


// Constructors 
// Construct from dictionary
mousse::snapParameters::snapParameters(const dictionary& dict)
:
  nSmoothPatch_{readLabel(dict.lookup("nSmoothPatch"))},
  snapTol_{readScalar(dict.lookup("tolerance"))},
  nSmoothDispl_{readLabel(dict.lookup("nSolveIter"))},
  nSnap_{readLabel(dict.lookup("nRelaxIter"))},
  nFeatureSnap_{dict.lookupOrDefault("nFeatureSnapIter", -1)},
  explicitFeatureSnap_{dict.lookupOrDefault("explicitFeatureSnap", true)},
  implicitFeatureSnap_{dict.lookupOrDefault("implicitFeatureSnap", false)},
  multiRegionFeatureSnap_
  {
    dict.lookupOrDefault("multiRegionFeatureSnap", false)
  },
  detectNearSurfacesSnap_
  {
    dict.lookupOrDefault("detectNearSurfacesSnap", true)
  }
{}

