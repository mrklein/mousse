// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_film_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
inline const mousse::dimensionedVector& surfaceFilmModel::g() const
{
  return g_;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
