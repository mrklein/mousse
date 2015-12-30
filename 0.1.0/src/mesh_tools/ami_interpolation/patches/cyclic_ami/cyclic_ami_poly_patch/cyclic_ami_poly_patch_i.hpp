// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::word& mousse::cyclicAMIPolyPatch::neighbPatchName() const
{
  if (nbrPatchName_.empty())
  {
    // Try and use patchGroup to find samplePatch and sampleRegion
    label patchID = coupleGroup_.findOtherPatchID(*this);
    nbrPatchName_ = boundaryMesh()[patchID].name();
  }
  return nbrPatchName_;
}
inline const mousse::vector& mousse::cyclicAMIPolyPatch::rotationAxis() const
{
  return rotationAxis_;
}
inline const mousse::point& mousse::cyclicAMIPolyPatch::rotationCentre() const
{
  return rotationCentre_;
}
inline const mousse::vector& mousse::cyclicAMIPolyPatch::separationVector() const
{
  return separationVector_;
}
