// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_radial.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(linearRadial, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(extrudeModel, linearRadial, dictionary);
// Constructors 
linearRadial::linearRadial(const dictionary& dict)
:
  extrudeModel(typeName, dict),
  R_(readScalar(coeffDict_.lookup("R"))),
  Rsurface_(coeffDict_.lookupOrDefault<scalar>("Rsurface", -1))
{}
// Destructor 
linearRadial::~linearRadial()
{}
// Operators
point linearRadial::operator()
(
  const point& surfacePoint,
  const vector& /*surfaceNormal*/,
  const label layer
) const
{
  // radius of the surface
  scalar rs = mag(surfacePoint);
  vector rsHat = surfacePoint/rs;
  if (Rsurface_ >= 0) rs = Rsurface_;
  scalar r = rs + (R_ - rs)*sumThickness(layer);
  return r*rsHat;
}
}  // namespace extrudeModels
}  // namespace mousse
