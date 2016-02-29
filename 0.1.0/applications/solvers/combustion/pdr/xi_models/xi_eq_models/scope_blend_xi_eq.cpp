// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scope_blend_xi_eq.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiEqModels
{
DEFINE_TYPE_NAME_AND_DEBUG(SCOPEBlend, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiEqModel, SCOPEBlend, dictionary);
}
}
// Constructors 
mousse::XiEqModels::SCOPEBlend::SCOPEBlend
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModel{XiEqProperties, thermo, turbulence, Su},
  XiEqModelL_
  {
    XiEqModel::New
    (
      XiEqModelCoeffs_.subDict("XiEqModelL"),
      thermo,
      turbulence,
      Su
    )
  },
  XiEqModelH_
  {
    XiEqModel::New
    (
      XiEqModelCoeffs_.subDict("XiEqModelH"),
      thermo,
      turbulence,
      Su
    )
  }
{}
// Destructor 
mousse::XiEqModels::SCOPEBlend::~SCOPEBlend()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiEqModels::SCOPEBlend::XiEq() const
{
  return pow
  (
    pow4(1.0/XiEqModelL_->XiEq()) + pow4(1.0/XiEqModelH_->XiEq()),
    -0.25
  );
}
