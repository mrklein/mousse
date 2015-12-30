// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::mappedPatchBase::sampleMode&
mousse::mappedPatchBase::mode() const
{
  return mode_;
}
inline const mousse::word& mousse::mappedPatchBase::sampleRegion() const
{
  if (sampleRegion_.empty())
  {
    if (!coupleGroup_.valid())
    {
      FatalErrorIn("mappedPatchBase::sampleRegion()")
        << "Supply either a regionName or a coupleGroup"
        << " for patch " << patch_.name()
        << " in region " << patch_.boundaryMesh().mesh().name()
        << exit(FatalError);
    }
    // Try and use patchGroup to find samplePatch and sampleRegion
    label samplePatchID = coupleGroup_.findOtherPatchID
    (
      patch_,
      sampleRegion_
    );
    samplePatch_ = sampleMesh().boundaryMesh()[samplePatchID].name();
  }
  return sampleRegion_;
}
inline const mousse::word& mousse::mappedPatchBase::samplePatch() const
{
  if (samplePatch_.empty())
  {
    if (!coupleGroup_.valid())
    {
      FatalErrorIn("mappedPatchBase::samplePolyPatch()")
        << "Supply either a patchName or a coupleGroup"
        << " for patch " << patch_.name()
        << " in region " << patch_.boundaryMesh().mesh().name()
        << exit(FatalError);
    }
    // Try and use patchGroup to find samplePatch and sampleRegion
    label samplePatchID = coupleGroup_.findOtherPatchID
    (
      patch_,
      sampleRegion_
    );
    samplePatch_ = sampleMesh().boundaryMesh()[samplePatchID].name();
  }
  return samplePatch_;
}
inline const mousse::word& mousse::mappedPatchBase::coupleGroup() const
{
  return coupleGroup_.name();
}
inline mousse::label mousse::mappedPatchBase::sampleSize() const
{
  switch (mode_)
  {
    case NEARESTPATCHFACEAMI:
    {
      return samplePolyPatch().size();
    }
    case NEARESTCELL:
    {
      return sampleMesh().nCells();
    }
    case NEARESTPATCHFACE:
    {
      return samplePolyPatch().size();
    }
    case NEARESTPATCHPOINT:
    {
      return samplePolyPatch().nPoints();
    }
    case NEARESTFACE:
    {
      const polyMesh& mesh = sampleMesh();
      return mesh.nFaces() - mesh.nInternalFaces();
    }
    default:
    {
      FatalErrorIn("mappedPatchBase::sampleSize()")
        << "problem." << abort(FatalError);
      return -1;
    }
  }
}
inline const mousse::vector& mousse::mappedPatchBase::offset() const
{
  return offset_;
}
inline const mousse::vectorField& mousse::mappedPatchBase::offsets() const
{
  return offsets_;
}
inline bool mousse::mappedPatchBase::sameRegion() const
{
  return sameRegion_;
}
inline const mousse::mapDistribute& mousse::mappedPatchBase::map() const
{
  if (mapPtr_.empty())
  {
    calcMapping();
  }
  return mapPtr_();
}
inline const mousse::AMIPatchToPatchInterpolation& mousse::mappedPatchBase::AMI
(
  bool forceUpdate
) const
{
  if (forceUpdate || AMIPtr_.empty())
  {
    calcAMI();
  }
  return AMIPtr_();
}
