// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "barotropic_compressibility_model.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(barotropicCompressibilityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(barotropicCompressibilityModel, dictionary);

}


// Constructors 
mousse::barotropicCompressibilityModel::barotropicCompressibilityModel
(
  const dictionary& compressibilityProperties,
  const volScalarField& gamma,
  const word& psiName
)
:
  compressibilityProperties_{compressibilityProperties},
  psi_
  {
    {
      psiName,
      gamma.mesh().time().timeName(),
      gamma.mesh()
    },
    gamma.mesh(),
    {psiName, dimensionSet(0, -2, 2, 0, 0), 0}
  },
  gamma_{gamma}
{}


// Member Functions 
bool mousse::barotropicCompressibilityModel::read
(
  const dictionary& compressibilityProperties
)
{
  compressibilityProperties_ = compressibilityProperties;
  return true;
}

