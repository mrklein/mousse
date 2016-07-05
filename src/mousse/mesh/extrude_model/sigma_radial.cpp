// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sigma_radial.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace extrudeModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(sigmaRadial, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(extrudeModel, sigmaRadial, dictionary);

// Constructors 
sigmaRadial::sigmaRadial(const dictionary& dict)
:
  extrudeModel{typeName, dict},
  RTbyg_{readScalar(coeffDict_.lookup("RTbyg"))},
  pRef_{readScalar(coeffDict_.lookup("pRef"))},
  pStrat_{readScalar(coeffDict_.lookup("pStrat"))}
{
  if (mag(expansionRatio() - 1.0) > SMALL) {
    WARNING_IN("sigmaRadial::sigmaRadial(const dictionary&)")
      << "Ignoring expansionRatio setting." << endl;
  }
}


// Destructor 
sigmaRadial::~sigmaRadial()
{}


// Operators
point sigmaRadial::operator()
(
  const point& surfacePoint,
  const vector& /*surfaceNormal*/,
  const label layer
) const
{
  // radius of the surface
  scalar rs = mag(surfacePoint);
  vector rsHat = surfacePoint/rs;
  scalar p = pRef_ - layer*(pRef_ - pStrat_)/nLayers_;
  scalar r = rs - RTbyg_*log(p/pRef_);
  return r*rsHat;
}

}  // namespace extrudeModels
}  // namespace mousse

