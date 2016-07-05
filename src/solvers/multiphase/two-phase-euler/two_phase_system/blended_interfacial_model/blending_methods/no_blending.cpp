// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_blending.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace blendingMethods {
DEFINE_TYPE_NAME_AND_DEBUG(noBlending, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  blendingMethod,
  noBlending,
  dictionary
);
}
}
// Constructors 
mousse::blendingMethods::noBlending::noBlending
(
  const dictionary& dict,
  const wordList& /*phaseNames*/
)
:
  blendingMethod{dict},
  continuousPhase_{dict.lookup("continuousPhase")}
{}
// Destructor 
mousse::blendingMethods::noBlending::~noBlending()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::noBlending::f1
(
  const phaseModel& phase1,
  const phaseModel& phase2
) const
{
  const fvMesh& mesh(phase1.mesh());
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "f",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        {"f", dimless, static_cast<double>(phase2.name() != continuousPhase_)}
      }
    };
}
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::noBlending::f2
(
  const phaseModel& phase1,
  const phaseModel& /*phase2*/
) const
{
  const fvMesh& mesh = phase1.mesh();
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "f",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        {"f", dimless, static_cast<double>(phase1.name() == continuousPhase_)}
      }
    };
}
