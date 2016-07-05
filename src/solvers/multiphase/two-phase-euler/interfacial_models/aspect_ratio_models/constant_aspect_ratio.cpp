// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_aspect_ratio.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace aspectRatioModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantAspectRatio, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  aspectRatioModel,
  constantAspectRatio,
  dictionary
);
}
}
// Constructors 
mousse::aspectRatioModels::constantAspectRatio::constantAspectRatio
(
  const dictionary& dict,
  const phasePair& pair
)
:
  aspectRatioModel{dict, pair},
  E0_{"E0", dimless, dict}
{}
// Destructor 
mousse::aspectRatioModels::constantAspectRatio::~constantAspectRatio()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::aspectRatioModels::constantAspectRatio::E() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "zero",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        E0_
      }
    };
}
