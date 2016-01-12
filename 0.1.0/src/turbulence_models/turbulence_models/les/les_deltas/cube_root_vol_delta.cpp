// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cube_root_vol_delta.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace LESModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(cubeRootVolDelta, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(LESdelta, cubeRootVolDelta, dictionary);
}
}
// Private Member Functions 
void mousse::LESModels::cubeRootVolDelta::calcDelta()
{
  const fvMesh& mesh = turbulenceModel_.mesh();
  label nD = mesh.nGeometricD();
  if (nD == 3)
  {
    delta_.internalField() = deltaCoeff_*pow(mesh.V(), 1.0/3.0);
  }
  else if (nD == 2)
  {
    WARNING_IN("cubeRootVolDelta::calcDelta()")
      << "Case is 2D, LES is not strictly applicable\n"
      << endl;
    const Vector<label>& directions = mesh.geometricD();
    scalar thickness = 0.0;
    for (direction dir=0; dir<directions.nComponents; dir++)
    {
      if (directions[dir] == -1)
      {
        thickness = mesh.bounds().span()[dir];
        break;
      }
    }
    delta_.internalField() = deltaCoeff_*sqrt(mesh.V()/thickness);
  }
  else
  {
    FATAL_ERROR_IN("cubeRootVolDelta::calcDelta()")
      << "Case is not 3D or 2D, LES is not applicable"
      << exit(FatalError);
  }
}
// Constructors 
mousse::LESModels::cubeRootVolDelta::cubeRootVolDelta
(
  const word& name,
  const turbulenceModel& turbulence,
  const dictionary& dict
)
:
  LESdelta(name, turbulence),
  deltaCoeff_
  (
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("deltaCoeff", 1)
  )
{
  calcDelta();
}
// Member Functions 
void mousse::LESModels::cubeRootVolDelta::read(const dictionary& dict)
{
  dict.subDict(type() + "Coeffs").readIfPresent<scalar>
  (
    "deltaCoeff",
    deltaCoeff_
  );
  calcDelta();
}
void mousse::LESModels::cubeRootVolDelta::correct()
{
  if (turbulenceModel_.mesh().changing())
  {
    calcDelta();
  }
}
