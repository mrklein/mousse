// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lun_pressure.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace granularPressureModels {
DEFINE_TYPE_NAME_AND_DEBUG(Lun, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  granularPressureModel,
  Lun,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::granularPressureModels::Lun::Lun
(
  const dictionary& dict
)
:
  granularPressureModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::granularPressureModels::Lun::~Lun()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::granularPressureModels::Lun::granularPressureCoeff
(
  const volScalarField& alpha1,
  const volScalarField& g0,
  const volScalarField& rho1,
  const dimensionedScalar& e
) const
{
  return rho1*alpha1*(1.0 + 2.0*(1.0 + e)*alpha1*g0);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::granularPressureModels::Lun::
granularPressureCoeffPrime
(
  const volScalarField& alpha1,
  const volScalarField& g0,
  const volScalarField& g0prime,
  const volScalarField& rho1,
  const dimensionedScalar& e
) const
{
  return rho1*(1.0 + alpha1*(1.0 + e)*(4.0*g0 + 2.0*g0prime*alpha1));
}
