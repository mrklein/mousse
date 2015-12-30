// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pyrolysis_model.hpp"
inline const mousse::Switch&
mousse::pyrolysisModels::pyrolysisModel::active() const
{
  return active_;
}
inline const mousse::dictionary&
mousse::pyrolysisModels::pyrolysisModel::coeffs() const
{
  return coeffs_;
}
