// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "plane_extrusion.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(plane, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(extrudeModel, plane, dictionary);
// Constructors 
plane::plane(const dictionary& dict)
:
  linearNormal(dict)
{
  if (nLayers_ != 1)
  {
    IO_WARNING_IN("plane::plane(const dictionary& dict)", dict)
      << "Expected nLayers (if specified) to be 1"
      << endl;
    nLayers_ = 1;
  }
}
// Destructor 
plane::~plane()
{}
}  // namespace extrudeModels
}  // namespace mousse
