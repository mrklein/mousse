// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_virtual_mass_coefficient.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace virtualMassModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantVirtualMassCoefficient, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  virtualMassModel,
  constantVirtualMassCoefficient,
  dictionary
);
}
}
// Constructors 
mousse::virtualMassModels::constantVirtualMassCoefficient::
constantVirtualMassCoefficient
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  virtualMassModel{dict, pair, registerObject},
  Cvm_{"Cvm", dimless, dict}
{}
// Destructor 
mousse::virtualMassModels::constantVirtualMassCoefficient::
~constantVirtualMassCoefficient()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::virtualMassModels::constantVirtualMassCoefficient::Cvm() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "Cvm",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      Cvm_
    }
  };
}
