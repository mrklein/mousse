// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inter_region_option.hpp"


// Member Functions 
bool mousse::fv::interRegionOption::read(const dictionary& dict)
{
  if (option::read(dict)) {
    return true;
  }
  return false;
}
