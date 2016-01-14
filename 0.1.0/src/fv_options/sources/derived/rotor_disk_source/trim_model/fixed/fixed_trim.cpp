// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_trim.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "unit_conversion.hpp"
#include "mathematical_constants.hpp"
using namespace mousse::constant;
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(fixedTrim, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(trimModel, fixedTrim, dictionary);
}
// Constructors 
mousse::fixedTrim::fixedTrim
(
  const fv::rotorDiskSource& rotor,
  const dictionary& dict
)
:
  trimModel(rotor, dict, typeName),
  thetag_(rotor.cells().size(), 0.0)
{
  read(dict);
}
// Destructor 
mousse::fixedTrim::~fixedTrim()
{}
// Member Functions 
void mousse::fixedTrim::read(const dictionary& dict)
{
  trimModel::read(dict);
  scalar theta0 = degToRad(readScalar(coeffs_.lookup("theta0")));
  scalar theta1c = degToRad(readScalar(coeffs_.lookup("theta1c")));
  scalar theta1s = degToRad(readScalar(coeffs_.lookup("theta1s")));
  const List<point>& x = rotor_.x();
  FOR_ALL(thetag_, i)
  {
    scalar psi = x[i].y();
    thetag_[i] = theta0 + theta1c*cos(psi) + theta1s*sin(psi);
  }
}
mousse::tmp<mousse::scalarField> mousse::fixedTrim::thetag() const
{
  return tmp<scalarField>(thetag_);
}
void mousse::fixedTrim::correct
(
  const vectorField& /*U*/,
  vectorField& /*force*/
)
{
  // do nothing
}
void mousse::fixedTrim::correct
(
  const volScalarField /*rho*/,
  const vectorField& /*U*/,
  vectorField& /*force*/)
{
  // do nothing
}
