// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "van_driest_delta.hpp"
#include "wall_fv_patch.hpp"
#include "wall_dist_data.hpp"
#include "wall_point_y_plus.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace LESModels {

DEFINE_TYPE_NAME_AND_DEBUG(vanDriestDelta, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(LESdelta, vanDriestDelta, dictionary);

}
}


// Private Member Functions 
void mousse::LESModels::vanDriestDelta::calcDelta()
{
  const fvMesh& mesh = turbulenceModel_.mesh();
  const volVectorField& U = turbulenceModel_.U();
  const tmp<volScalarField> tnu = turbulenceModel_.nu();
  const volScalarField& nu = tnu();
  tmp<volScalarField> nuSgs = turbulenceModel_.nut();
  volScalarField ystar
  {
    {
      "ystar",
      mesh.time().constant(),
      mesh
    },
    mesh,
    {"ystar", dimLength, GREAT}
  };
  const fvPatchList& patches = mesh.boundary();
  FOR_ALL(patches, patchi) {
    if (isA<wallFvPatch>(patches[patchi])) {
      const fvPatchVectorField& Uw = U.boundaryField()[patchi];
      const scalarField& nuw = nu.boundaryField()[patchi];
      const scalarField& nuSgsw = nuSgs().boundaryField()[patchi];
      ystar.boundaryField()[patchi] =
        nuw/sqrt((nuw + nuSgsw)*mag(Uw.snGrad()) + VSMALL);
    }
  }
  scalar cutOff = wallPointYPlus::yPlusCutOff;
  wallPointYPlus::yPlusCutOff = 500;
  wallDistData<wallPointYPlus> y{mesh, ystar};
  wallPointYPlus::yPlusCutOff = cutOff;
  delta_ =
    min(static_cast<const volScalarField&>(geometricDelta_()),
        (kappa_/Cdelta_)*((scalar(1) + SMALL) - exp(-y/ystar/Aplus_))*y);
}


// Constructors 
mousse::LESModels::vanDriestDelta::vanDriestDelta
(
  const word& name,
  const turbulenceModel& turbulence,
  const dictionary& dict
)
:
  LESdelta{name, turbulence},
  geometricDelta_
  {
    LESdelta::New
    (
      IOobject::groupName("geometricDelta", turbulence.U().group()),
      turbulence,
      dict.subDict(type() + "Coeffs")
    )
  },
  kappa_{dict.lookupOrDefault<scalar>("kappa", 0.41)},
  Aplus_
  {
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("Aplus", 26.0)
  },
  Cdelta_
  {
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("Cdelta", 0.158)
  },
  calcInterval_
  {
    dict.subDict(type() + "Coeffs").lookupOrDefault<label>
    (
      "calcInterval",
      1
    )
  }
{
  delta_ = geometricDelta_();
}


// Member Functions 
void mousse::LESModels::vanDriestDelta::read(const dictionary& dict)
{
  const dictionary& coeffsDict(dict.subDict(type() + "Coeffs"));
  geometricDelta_().read(coeffsDict);
  dict.readIfPresent<scalar>("kappa", kappa_);
  coeffsDict.readIfPresent<scalar>("Aplus", Aplus_);
  coeffsDict.readIfPresent<scalar>("Cdelta", Cdelta_);
  coeffsDict.readIfPresent<label>("calcInterval", calcInterval_);
  calcDelta();
}


void mousse::LESModels::vanDriestDelta::correct()
{
  if (turbulenceModel_.mesh().time().timeIndex() % calcInterval_ != 0)
    return;
  geometricDelta_().correct();
  calcDelta();
}

