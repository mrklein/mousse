// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::scalar mousse::fv::cellSetOption::timeStart() const
{
  return timeStart_;
}
inline mousse::scalar mousse::fv::cellSetOption::duration() const
{
  return duration_;
}
inline bool mousse::fv::cellSetOption::inTimeLimits(const scalar time) const
{
  return
  (
    (timeStart_ < 0)
  ||
    (
      (mesh_.time().value() >= timeStart_)
    && (mesh_.time().value() <= (timeStart_ + duration_))
    )
  );
}
inline const mousse::fv::cellSetOption::selectionModeType&
mousse::fv::cellSetOption::selectionMode() const
{
  return selectionMode_;
}
inline const mousse::word& mousse::fv::cellSetOption::cellSetName() const
{
  return cellSetName_;
}
inline mousse::scalar mousse::fv::cellSetOption::V() const
{
  return V_;
}
inline const mousse::labelList& mousse::fv::cellSetOption::cells() const
{
  return cells_;
}
inline mousse::scalar& mousse::fv::cellSetOption::timeStart()
{
  return timeStart_;
}
inline mousse::scalar& mousse::fv::cellSetOption::duration()
{
  return duration_;
}
