// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "syamlal_rogers_o_brien_pressure.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace granularPressureModels {
DEFINE_TYPE_NAME_AND_DEBUG(SyamlalRogersOBrien, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  granularPressureModel,
  SyamlalRogersOBrien,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::granularPressureModels::SyamlalRogersOBrien::
SyamlalRogersOBrien
(
  const dictionary& dict
)
:
  granularPressureModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::granularPressureModels::SyamlalRogersOBrien::
~SyamlalRogersOBrien()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::granularPressureModels::SyamlalRogersOBrien::
granularPressureCoeff
(
  const volScalarField& alpha1,
  const volScalarField& g0,
  const volScalarField& rho1,
  const dimensionedScalar& e
) const
{
  return 2.0*rho1*(1.0 + e)*sqr(alpha1)*g0;
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::granularPressureModels::SyamlalRogersOBrien::
granularPressureCoeffPrime
(
  const volScalarField& alpha1,
  const volScalarField& g0,
  const volScalarField& g0prime,
  const volScalarField& rho1,
  const dimensionedScalar& e
) const
{
  return rho1*alpha1*(1.0 + e)*(4.0*g0 + 2.0*g0prime*alpha1);
}
