// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "enriched_patch.hpp"
#include "demand_driven_data.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(enrichedPatch, 0);
}
// Private Member Functions 
void mousse::enrichedPatch::calcMeshPoints() const
{
  if (meshPointsPtr_)
  {
    FatalErrorIn("void enrichedPatch::calcMeshPoints() const")
      << "Mesh points already calculated."
      << abort(FatalError);
  }
  meshPointsPtr_ = new labelList(pointMap().toc());
  labelList& mp = *meshPointsPtr_;
  sort(mp);
}
void mousse::enrichedPatch::calcLocalFaces() const
{
  if (localFacesPtr_)
  {
    FatalErrorIn("void enrichedPatch::calcLocalFaces() const")
      << "Local faces already calculated."
      << abort(FatalError);
  }
  // Invert mesh points and renumber faces using it
  const labelList& mp = meshPoints();
  Map<label> mpLookup(2*mp.size());
  forAll(mp, mpI)
  {
    mpLookup.insert(mp[mpI], mpI);
  }
  const faceList& faces = enrichedFaces();
  localFacesPtr_ = new faceList(faces.size());
  faceList& lf = *localFacesPtr_;
  forAll(faces, faceI)
  {
    const face& f = faces[faceI];
    face& curlf = lf[faceI];
    curlf.setSize(f.size());
    forAll(f, pointI)
    {
      curlf[pointI] = mpLookup.find(f[pointI])();
    }
  }
}
void mousse::enrichedPatch::calcLocalPoints() const
{
  if (localPointsPtr_)
  {
    FatalErrorIn("void enrichedPatch::calcLocalPoints() const")
      << "Local points already calculated."
      << abort(FatalError);
  }
  const labelList& mp = meshPoints();
  localPointsPtr_ = new pointField(mp.size());
  pointField& lp = *localPointsPtr_;
  forAll(lp, i)
  {
    lp[i] = pointMap().find(mp[i])();
  }
}
void mousse::enrichedPatch::clearOut()
{
  deleteDemandDrivenData(enrichedFacesPtr_);
  deleteDemandDrivenData(meshPointsPtr_);
  deleteDemandDrivenData(localFacesPtr_);
  deleteDemandDrivenData(localPointsPtr_);
  deleteDemandDrivenData(pointPointsPtr_);
  deleteDemandDrivenData(masterPointFacesPtr_);
  clearCutFaces();
}
// Constructors 
// Construct from components
mousse::enrichedPatch::enrichedPatch
(
  const primitiveFacePatch& masterPatch,
  const primitiveFacePatch& slavePatch,
  const labelList& slavePointPointHits,
  const labelList& slavePointEdgeHits,
  const List<objectHit>& slavePointFaceHits
)
:
  masterPatch_(masterPatch),
  slavePatch_(slavePatch),
  pointMap_
  (
    masterPatch_.meshPoints().size()
   + slavePatch_.meshPoints().size()
  ),
  pointMapComplete_(false),
  pointMergeMap_(2*slavePatch_.meshPoints().size()),
  slavePointPointHits_(slavePointPointHits),
  slavePointEdgeHits_(slavePointEdgeHits),
  slavePointFaceHits_(slavePointFaceHits),
  enrichedFacesPtr_(NULL),
  meshPointsPtr_(NULL),
  localFacesPtr_(NULL),
  localPointsPtr_(NULL),
  pointPointsPtr_(NULL),
  masterPointFacesPtr_(NULL),
  cutFacesPtr_(NULL),
  cutFaceMasterPtr_(NULL),
  cutFaceSlavePtr_(NULL)
{}
// Destructor 
mousse::enrichedPatch::~enrichedPatch()
{
  clearOut();
}
// Member Functions 
const mousse::labelList& mousse::enrichedPatch::meshPoints() const
{
  if (!meshPointsPtr_)
  {
    calcMeshPoints();
  }
  return *meshPointsPtr_;
}
const mousse::faceList& mousse::enrichedPatch::localFaces() const
{
  if (!localFacesPtr_)
  {
    calcLocalFaces();
  }
  return *localFacesPtr_;
}
const mousse::pointField& mousse::enrichedPatch::localPoints() const
{
  if (!localPointsPtr_)
  {
    calcLocalPoints();
  }
  return *localPointsPtr_;
}
const mousse::labelListList& mousse::enrichedPatch::pointPoints() const
{
  if (!pointPointsPtr_)
  {
    calcPointPoints();
  }
  return *pointPointsPtr_;
}
bool mousse::enrichedPatch::checkSupport() const
{
  const faceList& faces = enrichedFaces();
  bool error = false;
  forAll(faces, faceI)
  {
    const face& curFace = faces[faceI];
    forAll(curFace, pointI)
    {
      if (!pointMap().found(curFace[pointI]))
      {
        WarningIn("void enrichedPatch::checkSupport()")
          << "Point " << pointI << " of face " << faceI
          << " global point index: " << curFace[pointI]
          << " not supported in point map.  This is not allowed."
          << endl;
        error = true;
      }
    }
  }
  return error;
}
void mousse::enrichedPatch::writeOBJ(const fileName& fName) const
{
  OFstream str(fName);
  const pointField& lp = localPoints();
  forAll(lp, pointI)
  {
    meshTools::writeOBJ(str, lp[pointI]);
  }
  const faceList& faces = localFaces();
  forAll(faces, faceI)
  {
    const face& f = faces[faceI];
    str << 'f';
    forAll(f, fp)
    {
      str << ' ' << f[fp]+1;
    }
    str << nl;
  }
}
// Member Operators 
// Friend Functions 
// Friend Operators
