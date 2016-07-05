// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_virtual_mass.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace virtualMassModels {
DEFINE_TYPE_NAME_AND_DEBUG(noVirtualMass, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(virtualMassModel, noVirtualMass, dictionary);
}
}
// Constructors 
mousse::virtualMassModels::noVirtualMass::noVirtualMass
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  virtualMassModel{dict, pair, registerObject}
{}
// Destructor 
mousse::virtualMassModels::noVirtualMass::~noVirtualMass()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::virtualMassModels::noVirtualMass::Cvm() const
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
        {"zero", dimless, 0}
      }
    };
}
mousse::tmp<mousse::volScalarField>
mousse::virtualMassModels::noVirtualMass::K() const
{
  return Cvm()*dimensionedScalar{"zero", dimDensity, 0};
}
