// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
inline const surfaceFilmModel& filmSubModelBase::owner() const
{
  return owner_;
}
inline surfaceFilmModel& filmSubModelBase::owner()
{
  return owner_;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
