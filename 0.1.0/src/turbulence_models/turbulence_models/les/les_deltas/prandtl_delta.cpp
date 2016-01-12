// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "prandtl_delta.hpp"
#include "wall_dist.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace LESModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(PrandtlDelta, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(LESdelta, PrandtlDelta, dictionary);
}
}
// Private Member Functions 
void mousse::LESModels::PrandtlDelta::calcDelta()
{
  delta_ = min
  (
    static_cast<const volScalarField&>(geometricDelta_()),
    (kappa_/Cdelta_)*wallDist::New(turbulenceModel_.mesh()).y()
  );
}
// Constructors 
mousse::LESModels::PrandtlDelta::PrandtlDelta
(
  const word& name,
  const turbulenceModel& turbulence,
  const dictionary& dict
)
:
  LESdelta(name, turbulence),
  geometricDelta_
  (
    LESdelta::New
    (
      name,
      turbulence,
      dict.subDict(type() + "Coeffs")
    )
  ),
  kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
  Cdelta_
  (
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("Cdelta", 0.158)
  )
{
  calcDelta();
}
// Member Functions 
void mousse::LESModels::PrandtlDelta::read(const dictionary& dict)
{
  const dictionary& coeffDict(dict.subDict(type() + "Coeffs"));
  geometricDelta_().read(coeffDict);
  dict.readIfPresent<scalar>("kappa", kappa_);
  coeffDict.readIfPresent<scalar>("Cdelta", Cdelta_);
  calcDelta();
}
void mousse::LESModels::PrandtlDelta::correct()
{
  geometricDelta_().correct();
  if (turbulenceModel_.mesh().changing())
  {
    calcDelta();
  }
}
