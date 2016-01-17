// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extrude_model.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(extrudeModel, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(extrudeModel, dictionary);
}
// Constructors 
mousse::extrudeModel::extrudeModel
(
  const word& modelType,
  const dictionary& dict
)
:
  nLayers_(dict.lookupOrDefault<label>("nLayers", 1)),
  expansionRatio_(dict.lookupOrDefault<scalar>("expansionRatio", 1)),
  dict_(dict),
  coeffDict_(dict.subDict(modelType + "Coeffs"))
{}
// Destructor 
mousse::extrudeModel::~extrudeModel()
{}
// Member Functions 
mousse::label mousse::extrudeModel::nLayers() const
{
  return nLayers_;
}
mousse::scalar mousse::extrudeModel::expansionRatio() const
{
  return expansionRatio_;
}
mousse::scalar mousse::extrudeModel::sumThickness(const label layer) const
{
  // 1+r+r^2+ .. +r^(n-1) = (1-r^n)/(1-r)
  if (mag(1.0-expansionRatio_) < SMALL)
  {
    return scalar(layer)/nLayers_;
  }
  else
  {
    return
      (1.0-pow(expansionRatio_, layer))
     / (1.0-pow(expansionRatio_, nLayers_));
  }
}
