// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_patch.hpp"
#include "dictionary.hpp"
#include "poly_mesh.hpp"
#include "poly_patch.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(sampledPatch, 0);
  addNamedToRunTimeSelectionTable(sampledSurface, sampledPatch, word, patch);
}
// Constructors 
mousse::sampledPatch::sampledPatch
(
  const word& name,
  const polyMesh& mesh,
  const wordReList& patchNames,
  const bool triangulate
)
:
  sampledSurface(name, mesh),
  patchNames_(patchNames),
  triangulate_(triangulate),
  needsUpdate_(true)
{}
mousse::sampledPatch::sampledPatch
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface(name, mesh, dict),
  patchNames_(dict.lookup("patches")),
  triangulate_(dict.lookupOrDefault("triangulate", false)),
  needsUpdate_(true)
{}
// Destructor 
mousse::sampledPatch::~sampledPatch()
{}
// Member Functions 
const mousse::labelList& mousse::sampledPatch::patchIDs() const
{
  if (patchIDs_.empty())
  {
    patchIDs_ = mesh().boundaryMesh().patchSet
    (
      patchNames_,
      false
    ).sortedToc();
  }
  return patchIDs_;
}
bool mousse::sampledPatch::needsUpdate() const
{
  return needsUpdate_;
}
bool mousse::sampledPatch::expire()
{
  // already marked as expired
  if (needsUpdate_)
  {
    return false;
  }
  sampledSurface::clearGeom();
  MeshStorage::clear();
  patchIDs_.clear();
  patchIndex_.clear();
  patchFaceLabels_.clear();
  patchStart_.clear();
  needsUpdate_ = true;
  return true;
}
bool mousse::sampledPatch::update()
{
  if (!needsUpdate_)
  {
    return false;
  }
  label sz = 0;
  forAll(patchIDs(), i)
  {
    label patchI = patchIDs()[i];
    const polyPatch& pp = mesh().boundaryMesh()[patchI];
    if (isA<emptyPolyPatch>(pp))
    {
      FatalErrorIn("sampledPatch::update()")
        << "Cannot sample an empty patch. Patch " << pp.name()
        << exit(FatalError);
    }
    sz += pp.size();
  }
  // For every face (or triangle) the originating patch and local face in the
  // patch.
  patchIndex_.setSize(sz);
  patchFaceLabels_.setSize(sz);
  patchStart_.setSize(patchIDs().size());
  labelList meshFaceLabels(sz);
  sz = 0;
  forAll(patchIDs(), i)
  {
    label patchI = patchIDs()[i];
    patchStart_[i] = sz;
    const polyPatch& pp = mesh().boundaryMesh()[patchI];
    forAll(pp, j)
    {
      patchIndex_[sz] = i;
      patchFaceLabels_[sz] = j;
      meshFaceLabels[sz] = pp.start()+j;
      sz++;
    }
  }
  indirectPrimitivePatch allPatches
  (
    IndirectList<face>(mesh().faces(), meshFaceLabels),
    mesh().points()
  );
  this->storedPoints() = allPatches.localPoints();
  this->storedFaces()  = allPatches.localFaces();
  // triangulate uses remapFaces()
  // - this is somewhat less efficient since it recopies the faces
  // that we just created, but we probably don't want to do this
  // too often anyhow.
  if (triangulate_)
  {
    MeshStorage::triangulate();
  }
  if (debug)
  {
    print(Pout);
    Pout<< endl;
  }
  needsUpdate_ = false;
  return true;
}
// remap action on triangulation
void mousse::sampledPatch::remapFaces(const labelUList& faceMap)
{
  // recalculate the cells cut
  if (notNull(faceMap) && faceMap.size())
  {
    MeshStorage::remapFaces(faceMap);
    patchFaceLabels_ = labelList
    (
      UIndirectList<label>(patchFaceLabels_, faceMap)
    );
    patchIndex_ = labelList
    (
      UIndirectList<label>(patchIndex_, faceMap)
    );
    // Redo patchStart.
    if (patchIndex_.size() > 0)
    {
      patchStart_[patchIndex_[0]] = 0;
      for (label i = 1; i < patchIndex_.size(); i++)
      {
        if (patchIndex_[i] != patchIndex_[i-1])
        {
          patchStart_[patchIndex_[i]] = i;
        }
      }
    }
  }
}
mousse::tmp<mousse::scalarField> mousse::sampledPatch::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField> mousse::sampledPatch::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPatch::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPatch::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField> mousse::sampledPatch::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField> mousse::sampledPatch::sample
(
  const surfaceScalarField& sField
) const
{
  return sampleField(sField);
}
mousse::tmp<mousse::vectorField> mousse::sampledPatch::sample
(
  const surfaceVectorField& sField
) const
{
  return sampleField(sField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPatch::sample
(
  const surfaceSphericalTensorField& sField
) const
{
  return sampleField(sField);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPatch::sample
(
  const surfaceSymmTensorField& sField
) const
{
  return sampleField(sField);
}
mousse::tmp<mousse::tensorField> mousse::sampledPatch::sample
(
  const surfaceTensorField& sField
) const
{
  return sampleField(sField);
}
mousse::tmp<mousse::scalarField> mousse::sampledPatch::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField> mousse::sampledPatch::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPatch::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPatch::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField> mousse::sampledPatch::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledPatch::print(Ostream& os) const
{
  os  << "sampledPatch: " << name() << " :"
    << "  patches:" << patchNames()
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}
