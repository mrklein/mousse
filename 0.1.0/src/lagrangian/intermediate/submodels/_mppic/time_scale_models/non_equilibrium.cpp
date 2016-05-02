// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "non_equilibrium.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace TimeScaleModels {

DEFINE_TYPE_NAME_AND_DEBUG(nonEquilibrium, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  TimeScaleModel,
  nonEquilibrium,
  dictionary
);

}
}


// Constructors 
mousse::TimeScaleModels::nonEquilibrium::nonEquilibrium
(
  const dictionary& dict
)
:
  TimeScaleModel{dict}
{}


mousse::TimeScaleModels::nonEquilibrium::nonEquilibrium
(
  const nonEquilibrium& hc
)
:
  TimeScaleModel{hc}
{}


// Destructor 
mousse::TimeScaleModels::nonEquilibrium::~nonEquilibrium()
{}


// Member Functions 
mousse::tmp<mousse::FieldField<mousse::Field, mousse::scalar> >
mousse::TimeScaleModels::nonEquilibrium::oneByTau
(
  const FieldField<Field, scalar>& alpha,
  const FieldField<Field, scalar>& /*r32*/,
  const FieldField<Field, scalar>& /*uSqr*/,
  const FieldField<Field, scalar>& f
) const
{
  static const scalar a =
    8.0*sqrt(2.0)/(3.0*constant::mathematical::pi)*0.25*(1.0 - e_*e_);
  return a*f*alphaPacked_/max(alphaPacked_ - alpha, SMALL);
}

