// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radial.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
defineTypeNameAndDebug(radial, 0);
addToRunTimeSelectionTable(extrudeModel, radial, dictionary);
// Constructors 
radial::radial(const dictionary& dict)
:
  extrudeModel(typeName, dict),
  R_(DataEntry<scalar>::New("R", coeffDict_))
{}
// Destructor 
radial::~radial()
{}
// Operators
point radial::operator()
(
  const point& surfacePoint,
  const vector& surfaceNormal,
  const label layer
) const
{
  // radius of the surface
  scalar rs = mag(surfacePoint);
  vector rsHat = surfacePoint/rs;
  scalar r = R_->value(layer);
Pout<< "** for layer " << layer << " r:" << r << endl;
  return r*rsHat;
}
}  // namespace extrudeModels
}  // namespace mousse
