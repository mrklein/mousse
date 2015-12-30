// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class ParticleType>
const mousse::polyMesh& mousse::InteractionLists<ParticleType>::mesh() const
{
  return mesh_;
}
template<class ParticleType>
const mousse::mapDistribute&
mousse::InteractionLists<ParticleType>::cellMap() const
{
  return cellMapPtr_();
}
template<class ParticleType>
const mousse::mapDistribute&
mousse::InteractionLists<ParticleType>::wallFaceMap() const
{
  return wallFaceMapPtr_();
}
template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::dil() const
{
  return dil_;
}
template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::dwfil() const
{
  return dwfil_;
}
template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::ril() const
{
  return ril_;
}
template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::rilInverse() const
{
  return rilInverse_;
}
template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::rwfil() const
{
  return rwfil_;
}
template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::rwfilInverse() const
{
  return rwfilInverse_;
}
template<class ParticleType>
const mousse::List<mousse::labelPair>&
mousse::InteractionLists<ParticleType>::cellIndexAndTransformToDistribute() const
{
  return cellIndexAndTransformToDistribute_;
}
template<class ParticleType>
const mousse::List<mousse::labelPair>&
mousse::InteractionLists<ParticleType>::
wallFaceIndexAndTransformToDistribute() const
{
  return wallFaceIndexAndTransformToDistribute_;
}
template<class ParticleType>
const mousse::List<mousse::referredWallFace>&
mousse::InteractionLists<ParticleType>::referredWallFaces() const
{
  return referredWallFaces_;
}
template<class ParticleType>
const mousse::word& mousse::InteractionLists<ParticleType>::UName() const
{
  return UName_;
}
template<class ParticleType>
const mousse::List<mousse::vector>&
mousse::InteractionLists<ParticleType>::referredWallData() const
{
  return referredWallData_;
}
template<class ParticleType>
const mousse::List<mousse::IDLList<ParticleType> >&
mousse::InteractionLists<ParticleType>::referredParticles() const
{
  return referredParticles_;
}
template<class ParticleType>
mousse::List<mousse::IDLList<ParticleType> >&
mousse::InteractionLists<ParticleType>::referredParticles()
{
  return referredParticles_;
}
