// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rpm.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mathematical_constants.hpp"
// Static Data Members
namespace mousse
{
namespace SRF
{
DEFINE_TYPE_NAME_AND_DEBUG(rpm, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  SRFModel,
  rpm,
  dictionary
);
}
}
// Constructors 
mousse::SRF::rpm::rpm
(
  const volVectorField& U
)
:
  SRFModel{typeName, U},
  rpm_{readScalar(SRFModelCoeffs_.lookup("rpm"))}
{
  // Initialise the angular velocity
  omega_.value() = axis_*rpm_*constant::mathematical::twoPi/60.0;
}
// Destructor 
mousse::SRF::rpm::~rpm()
{}
// Member Functions 
bool mousse::SRF::rpm::read()
{
  if (SRFModel::read())
  {
    // Re-read rpm
    SRFModelCoeffs_.lookup("rpm") >> rpm_;
    // Update angular velocity
    omega_.value() = axis_*rpm_*(constant::mathematical::twoPi/60.0);
    return true;
  }
  else
  {
    return false;
  }
}
