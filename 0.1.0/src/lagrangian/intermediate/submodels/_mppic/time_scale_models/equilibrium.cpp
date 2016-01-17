// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "equilibrium.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace TimeScaleModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(equilibrium, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    TimeScaleModel,
    equilibrium,
    dictionary
  );
}
}
// Constructors 
mousse::TimeScaleModels::equilibrium::equilibrium
(
  const dictionary& dict
)
:
  TimeScaleModel(dict)
{}
mousse::TimeScaleModels::equilibrium::equilibrium
(
  const equilibrium& hc
)
:
  TimeScaleModel(hc)
{}
// Destructor 
mousse::TimeScaleModels::equilibrium::~equilibrium()
{}
// Member Functions 
mousse::tmp<mousse::FieldField<mousse::Field, mousse::scalar> >
mousse::TimeScaleModels::equilibrium::oneByTau
(
  const FieldField<Field, scalar>& alpha,
  const FieldField<Field, scalar>& r32,
  const FieldField<Field, scalar>& uSqr,
  const FieldField<Field, scalar>& /*f*/
) const
{
  static const scalar a =
    16.0/sqrt(3.0*constant::mathematical::pi)
   *0.25*(1.0 - e_*e_);
  return
    a
   *alpha*sqrt(max(uSqr, scalar(0)))/max(r32, SMALL)
   *alphaPacked_/max(alphaPacked_ - alpha, SMALL);
}
