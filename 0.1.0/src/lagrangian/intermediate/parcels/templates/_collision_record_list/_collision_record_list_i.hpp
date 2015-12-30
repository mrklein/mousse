// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class PairType, class WallType>
const mousse::DynamicList<mousse::PairCollisionRecord<PairType> >&
mousse::CollisionRecordList<PairType, WallType>::pairRecords() const
{
  return pairRecords_;
}
template<class PairType, class WallType>
const mousse::DynamicList<mousse::WallCollisionRecord<WallType> >&
mousse::CollisionRecordList<PairType, WallType>::wallRecords() const
{
  return wallRecords_;
}
