// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_sector.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
defineTypeNameAndDebug(cyclicSector, 0);
addToRunTimeSelectionTable(extrudeModel, cyclicSector, dictionary);
// Constructors 
cyclicSector::cyclicSector(const dictionary& dict)
:
  sector(dict)
{}
// Destructor 
cyclicSector::~cyclicSector()
{}
}  // namespace extrudeModels
}  // namespace mousse
