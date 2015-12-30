// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
defineTypeNameAndDebug(wedge, 0);
addToRunTimeSelectionTable(extrudeModel, wedge, dictionary);
// Constructors 
wedge::wedge(const dictionary& dict)
:
  sector(dict)
{
  if (nLayers_ != 1)
  {
    IOWarningIn("wedge::wedge(const dictionary& dict)", dict)
      << "Expected nLayers (if specified) to be 1"
      << endl;
    nLayers_ = 1;
  }
}
// Destructor 
wedge::~wedge()
{}
}  // namespace extrudeModels
}  // namespace mousse
