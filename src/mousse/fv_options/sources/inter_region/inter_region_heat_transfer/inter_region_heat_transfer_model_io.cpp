// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inter_region_heat_transfer_model.hpp"


// Member Functions 
bool mousse::fv::interRegionHeatTransferModel::read(const dictionary& dict)
{
  if (interRegionOption::read(dict)) {
    return true;
  }
  return false;
}

