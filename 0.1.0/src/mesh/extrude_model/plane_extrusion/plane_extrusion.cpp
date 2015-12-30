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
defineTypeNameAndDebug(plane, 0);
addToRunTimeSelectionTable(extrudeModel, plane, dictionary);
// Constructors 
plane::plane(const dictionary& dict)
:
  linearNormal(dict)
{
  if (nLayers_ != 1)
  {
    IOWarningIn("plane::plane(const dictionary& dict)", dict)
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
