// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "enriched_patch.hpp"
#include "primitive_mesh.hpp"
#include "dynamic_list.hpp"
// Static Data Members
// Private Member Functions 
void mousse::enrichedPatch::calcPointPoints() const
{
  // Calculate point-point addressing
  if (pointPointsPtr_)
  {
    FatalErrorIn("void enrichedPatch::calcPointPoints() const")
      << "Point-point addressing already calculated."
      << abort(FatalError);
  }
  // Algorithm:
  // Go through all faces and add the previous and next point as the
  // neighbour for each point. While inserting points, reject the
  // duplicates (as every internal edge will be visited twice).
  List<DynamicList<label, primitiveMesh::edgesPerPoint_> >
    pp(meshPoints().size());
  const faceList& lf = localFaces();
  bool found = false;
  forAll(lf, faceI)
  {
    const face& curFace = lf[faceI];
    forAll(curFace, pointI)
    {
      DynamicList<label, primitiveMesh::edgesPerPoint_>&
        curPp = pp[curFace[pointI]];
      // Do next label
      label next = curFace.nextLabel(pointI);
      found = false;
      forAll(curPp, i)
      {
        if (curPp[i] == next)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        curPp.append(next);
      }
      // Do previous label
      label prev = curFace.prevLabel(pointI);
      found = false;
      forAll(curPp, i)
      {
        if (curPp[i] == prev)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        curPp.append(prev);
      }
    }
  }
  // Re-pack the list
  pointPointsPtr_ = new labelListList(pp.size());
  labelListList& ppAddr = *pointPointsPtr_;
  forAll(pp, pointI)
  {
    ppAddr[pointI].transfer(pp[pointI]);
  }
}
