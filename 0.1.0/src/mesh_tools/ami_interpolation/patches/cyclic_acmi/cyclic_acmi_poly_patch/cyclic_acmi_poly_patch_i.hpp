// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline void mousse::cyclicACMIPolyPatch::setUpdated(const bool flag) const
{
  updated_ = flag;
}
inline bool mousse::cyclicACMIPolyPatch::updated() const
{
  return updated_;
}
inline const mousse::vectorField& mousse::cyclicACMIPolyPatch::faceAreas0() const
{
  return faceAreas0_;
}
inline const mousse::word& mousse::cyclicACMIPolyPatch::nonOverlapPatchName() const
{
  return nonOverlapPatchName_;
}
inline const mousse::polyPatch& mousse::cyclicACMIPolyPatch::nonOverlapPatch() const
{
  // note: use nonOverlapPatchID() as opposed to patch name to initialise
  // demand-driven data
  return this->boundaryMesh()[nonOverlapPatchID()];
}
inline mousse::polyPatch& mousse::cyclicACMIPolyPatch::nonOverlapPatch()
{
  // note: use nonOverlapPatchID() as opposed to patch name to initialise
  // demand-driven data
  return const_cast<polyPatch&>(this->boundaryMesh()[nonOverlapPatchID()]);
}
inline const mousse::scalarField& mousse::cyclicACMIPolyPatch::mask() const
{
  if (owner())
  {
    return srcMask_;
  }
  else
  {
    return neighbPatch().tgtMask();
  }
}
