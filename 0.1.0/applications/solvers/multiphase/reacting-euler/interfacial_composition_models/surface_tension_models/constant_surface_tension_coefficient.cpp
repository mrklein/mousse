// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_surface_tension_coefficient.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace surfaceTensionModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantSurfaceTensionCoefficient, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  surfaceTensionModel,
  constantSurfaceTensionCoefficient,
  dictionary
);
}
}
// Constructors 
mousse::surfaceTensionModels::constantSurfaceTensionCoefficient::
constantSurfaceTensionCoefficient
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  surfaceTensionModel{dict, pair, registerObject},
  sigma_{"sigma", dimSigma, dict}
{}
// Destructor 
mousse::surfaceTensionModels::constantSurfaceTensionCoefficient::
~constantSurfaceTensionCoefficient()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::surfaceTensionModels::constantSurfaceTensionCoefficient::sigma() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "sigma",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      sigma_
    }
  };
}
