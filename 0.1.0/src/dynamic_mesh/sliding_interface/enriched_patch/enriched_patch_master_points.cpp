// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "enriched_patch.hpp"
#include "primitive_mesh.hpp"
#include "demand_driven_data.hpp"
#include "dynamic_list.hpp"
// Static Data Members
const mousse::label mousse::enrichedPatch::nFaceHits_ = 4;
// Private Member Functions 
void mousse::enrichedPatch::calcMasterPointFaces() const
{
  if (masterPointFacesPtr_)
  {
    FATAL_ERROR_IN("void enrichedPatch::calcMasterPointFaces() const")
      << "Master point face addressing already calculated."
      << abort(FatalError);
  }
  // Note:
  // Master point face addressing lists the master faces for all points
  // in the enriched patch support (if there are no master faces, which is
  // normal, the list will be empty).  The index represents the index of
  // the master face rather than the index from the enriched patch
  // Master face points lists the points of the enriched master face plus
  // points projected into the master face
  Map<DynamicList<label> > mpf(meshPoints().size());
  const faceList& ef = enrichedFaces();
  // Add the original face points
  FOR_ALL(masterPatch_, faceI)
  {
    const face& curFace = ef[faceI + slavePatch_.size()];
//         Pout<< "Cur face in pfAddr: " << curFace << endl;
    FOR_ALL(curFace, pointI)
    {
      Map<DynamicList<label> >::iterator mpfIter =
        mpf.find(curFace[pointI]);
      if (mpfIter == mpf.end())
      {
        // Not found, add new dynamic list
        mpf.insert
        (
          curFace[pointI],
          DynamicList<label>(primitiveMesh::facesPerPoint_)
        );
        // Iterator is invalidated - have to find again
        mpf.find(curFace[pointI])().append(faceI);
      }
      else
      {
        mpfIter().append(faceI);
      }
    }
  }
  // Add the projected points which hit the face
  const labelList& slaveMeshPoints = slavePatch_.meshPoints();
  FOR_ALL(slavePointFaceHits_, pointI)
  {
    if
    (
      slavePointPointHits_[pointI] < 0
    && slavePointEdgeHits_[pointI] < 0
    && slavePointFaceHits_[pointI].hit()
    )
    {
      // Get the index of projected point corresponding to this slave
      // point
      const label mergedSmp =
        pointMergeMap().find(slaveMeshPoints[pointI])();
      Map<DynamicList<label> >::iterator mpfIter =
        mpf.find(mergedSmp);
      if (mpfIter == mpf.end())
      {
        // Not found, add new dynamic list
        mpf.insert
        (
          mergedSmp,
          DynamicList<label>(primitiveMesh::facesPerPoint_)
        );
        // Iterator is invalidated - have to find again
        mpf.find(mergedSmp)().append
        (
          slavePointFaceHits_[pointI].hitObject()
        );
      }
      else
      {
        mpfIter().append(slavePointFaceHits_[pointI].hitObject());
      }
    }
  }
  // Re-pack dynamic lists into normal lists
  const labelList mpfToc = mpf.toc();
  masterPointFacesPtr_ = new Map<labelList>(2*mpfToc.size());
  Map<labelList>& masterPointFaceAddr = *masterPointFacesPtr_;
  FOR_ALL(mpfToc, mpfTocI)
  {
    labelList l;
    l.transfer(mpf.find(mpfToc[mpfTocI])());
    masterPointFaceAddr.insert(mpfToc[mpfTocI], l);
  }
  // Pout<< "masterPointFaceAddr: " << masterPointFaceAddr << endl;
}
// Member Functions 
const mousse::Map<mousse::labelList>& mousse::enrichedPatch::masterPointFaces() const
{
  if (!masterPointFacesPtr_)
  {
    calcMasterPointFaces();
  }
  return *masterPointFacesPtr_;
}
