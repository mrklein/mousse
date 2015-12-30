// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_normal.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace extrudeModels
{
// Static Data Members
defineTypeNameAndDebug(linearNormal, 0);
addToRunTimeSelectionTable(extrudeModel, linearNormal, dictionary);
// Constructors 
linearNormal::linearNormal(const dictionary& dict)
:
  extrudeModel(typeName, dict),
  thickness_(readScalar(coeffDict_.lookup("thickness"))),
  firstCellThickness_(0),
  layerPoints_(nLayers_)
{
  if (thickness_ <= 0)
  {
    FatalErrorIn("linearNormal(const dictionary&)")
      << "thickness should be positive : " << thickness_
      << exit(FatalError);
  }
  coeffDict_.readIfPresent("firstCellThickness", firstCellThickness_);
  if (firstCellThickness_ >= thickness_)
  {
    FatalErrorIn("linearNormal(const dictionary&)")
      << "firstCellThickness is larger than thickness"
      << exit(FatalError);
  }
  if (firstCellThickness_ > 0)
  {
    layerPoints_[0] = firstCellThickness_;
    for (label layerI = 1; layerI < nLayers_; layerI++)
    {
      layerPoints_[layerI] =
        (thickness_ - layerPoints_[0])
        *sumThickness(layerI) + layerPoints_[0];
    }
  }
  else
  {
    for (label layerI = 0; layerI < nLayers_; layerI++)
    {
      layerPoints_[layerI] = thickness_*sumThickness(layerI + 1);
    }
  }
}
// Destructor 
linearNormal::~linearNormal()
{}
// Operators
point linearNormal::operator()
(
  const point& surfacePoint,
  const vector& surfaceNormal,
  const label layer
) const
{
  if (layer == 0)
  {
    return surfacePoint;
  }
  else
  {
    return surfacePoint + layerPoints_[layer - 1]*surfaceNormal;
  }
}
}  // namespace extrudeModels
}  // namespace mousse
