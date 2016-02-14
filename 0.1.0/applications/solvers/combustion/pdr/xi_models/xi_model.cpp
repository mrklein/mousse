// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_model.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(XiModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(XiModel, dictionary);
}
// Constructors 
mousse::XiModel::XiModel
(
  const dictionary& XiProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su,
  const volScalarField& rho,
  const volScalarField& b,
  const surfaceScalarField& phi
)
:
  XiModelCoeffs_
  {
    XiProperties.subDict
    (
      word(XiProperties.lookup("XiModel")) + "Coeffs"
    )
  },
  thermo_{thermo},
  turbulence_{turbulence},
  Su_{Su},
  rho_{rho},
  b_{b},
  phi_{phi},
  Xi_
  {
    {
      "Xi",
      b.time().timeName(),
      b.db(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    b.mesh()
  }
{}
// Destructor 
mousse::XiModel::~XiModel()
{}
// Member Functions 
bool mousse::XiModel::read(const dictionary& XiProperties)
{
  XiModelCoeffs_ = XiProperties.subDict(type() + "Coeffs");
  return true;
}
