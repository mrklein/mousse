// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_direction.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace extrudeModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(linearDirection, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(extrudeModel, linearDirection, dictionary);


// Constructors 
linearDirection::linearDirection(const dictionary& dict)
:
  extrudeModel{typeName, dict},
  direction_{coeffDict_.lookup("direction")},
  thickness_{readScalar(coeffDict_.lookup("thickness"))}
{
  direction_ /= mag(direction_);
  if (thickness_ <= 0) {
    FATAL_ERROR_IN("linearDirection(const dictionary&)")
      << "thickness should be positive : " << thickness_
      << exit(FatalError);
  }
}


// Destructor 
linearDirection::~linearDirection()
{}


// Operators
point linearDirection::operator()
(
  const point& surfacePoint,
  const vector& /*surfaceNormal*/,
  const label layer
) const
{
  //scalar d = thickness_*layer/nLayers_;
  scalar d = thickness_*sumThickness(layer);
  return surfacePoint + d*direction_;
}

}  // namespace extrudeModels
}  // namespace mousse

