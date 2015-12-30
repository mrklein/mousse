// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::word& mousse::porosityModel::name() const
{
  return name_;
}
inline bool mousse::porosityModel::active() const
{
  return active_;
}
inline const mousse::labelList& mousse::porosityModel::cellZoneIDs() const
{
  return cellZoneIDs_;
}
