// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_g_model.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(XiGModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(XiGModel, dictionary);
}
// Constructors 
mousse::XiGModel::XiGModel
(
  const dictionary& XiGProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiGModelCoeffs_
  {
    XiGProperties.subDict
    (
      word{XiGProperties.lookup("XiGModel")} + "Coeffs"
    )
  },
  thermo_{thermo},
  turbulence_{turbulence},
  Su_{Su}
{}
// Destructor 
mousse::XiGModel::~XiGModel()
{}
// Member Functions 
bool mousse::XiGModel::read(const dictionary& XiGProperties)
{
  XiGModelCoeffs_ = XiGProperties.subDict(type() + "Coeffs");
  return true;
}
