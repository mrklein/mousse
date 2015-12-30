// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "isotropic.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace TimeScaleModels
{
  defineTypeNameAndDebug(isotropic, 0);
  addToRunTimeSelectionTable
  (
    TimeScaleModel,
    isotropic,
    dictionary
  );
}
}
// Constructors 
mousse::TimeScaleModels::isotropic::isotropic
(
  const dictionary& dict
)
:
  TimeScaleModel(dict)
{}
mousse::TimeScaleModels::isotropic::isotropic
(
  const isotropic& hc
)
:
  TimeScaleModel(hc)
{}
// Destructor 
mousse::TimeScaleModels::isotropic::~isotropic()
{}
// Member Functions 
mousse::tmp<mousse::FieldField<mousse::Field, mousse::scalar> >
mousse::TimeScaleModels::isotropic::oneByTau
(
  const FieldField<Field, scalar>& alpha,
  const FieldField<Field, scalar>& r32,
  const FieldField<Field, scalar>& uSqr,
  const FieldField<Field, scalar>& f
) const
{
  static const scalar a =
    8.0*sqrt(2.0)/(5.0*constant::mathematical::pi)
   *0.25*(3.0 - e_)*(1.0 + e_);
  
  return a*f*alphaPacked_/max(alphaPacked_ - alpha, SMALL);
}
