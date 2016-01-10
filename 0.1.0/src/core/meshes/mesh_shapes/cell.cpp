// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell.hpp"
#include "pyramid_point_face_ref.hpp"
// Static Data Members
const char* const mousse::cell::typeName = "cell";
// Member Functions 
mousse::labelList mousse::cell::labels(const faceUList& f) const
{
  // return the unordered list of vertex labels supporting the cell
  // count the maximum size of all vertices
  label maxVert = 0;
  const labelList& faces = *this;
  FOR_ALL(faces, faceI)
  {
    maxVert += f[faces[faceI]].size();
  }
  // set the fill-in list
  labelList p(maxVert);
  // in the first face there is no duplicates
  const labelList& first = f[faces[0]];
  FOR_ALL(first, pointI)
  {
    p[pointI] = first[pointI];
  }
  // re-use maxVert to count the real vertices
  maxVert = first.size();
  // go through the rest of the faces. For each vertex, check if the point is
  // already inserted (up to maxVert, which now carries the number of real
  // points. If not, add it at the end of the list.
  for (label faceI = 1; faceI < faces.size(); faceI++)
  {
    const labelList& curFace = f[faces[faceI]];
    FOR_ALL(curFace, pointI)
    {
      const label curPoint = curFace[pointI];
      bool found = false;
      for (label checkI = 0; checkI < maxVert; checkI++)
      {
        if (curPoint == p[checkI])
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        p[maxVert] = curPoint;
        maxVert++;
      }
    }
  }
  // reset the size of the list
  p.setSize(maxVert);
  return p;
}
mousse::pointField mousse::cell::points
(
  const faceUList& f,
  const pointField& meshPoints
) const
{
  labelList pointLabels = labels(f);
  pointField p(pointLabels.size());
  FOR_ALL(p, i)
  {
    p[i] = meshPoints[pointLabels[i]];
  }
  return p;
}
mousse::edgeList mousse::cell::edges(const faceUList& f) const
{
  // return the lisf of cell edges
  const labelList& curFaces = *this;
  // create a list of edges
  label maxNoEdges = 0;
  FOR_ALL(curFaces, faceI)
  {
    maxNoEdges += f[curFaces[faceI]].nEdges();
  }
  edgeList allEdges(maxNoEdges);
  label nEdges = 0;
  FOR_ALL(curFaces, faceI)
  {
    const edgeList curFaceEdges = f[curFaces[faceI]].edges();
    FOR_ALL(curFaceEdges, faceEdgeI)
    {
      const edge& curEdge = curFaceEdges[faceEdgeI];
      bool edgeFound = false;
      for (label addedEdgeI = 0; addedEdgeI < nEdges; addedEdgeI++)
      {
        if (allEdges[addedEdgeI] == curEdge)
        {
          edgeFound = true;
          break;
        }
      }
      if (!edgeFound)
      {
        // Add the new edge onto the list
        allEdges[nEdges] = curEdge;
        nEdges++;
      }
    }
  }
  allEdges.setSize(nEdges);
  return allEdges;
}
mousse::point mousse::cell::centre
(
  const pointField& p,
  const faceUList& f
) const
{
  // When one wants to access the cell centre and magnitude, the
  // functionality on the mesh level should be used in preference to the
  // functions provided here. They do not rely to the functionality
  // implemented here, provide additional checking and are more efficient.
  // The cell::centre and cell::mag functions may be removed in the future.
  // WARNING!
  // In the old version of the code, it was possible to establish whether any
  // of the pyramids had a negative volume, caused either by the poor
  // estimate of the cell centre or by the fact that the cell was defined
  // inside out. In the new description of the cell, this can only be
  // established with the reference to the owner-neighbour face-cell
  // relationship, which is not available on this level. Thus, all the
  // pyramids are believed to be positive with no checking.
  // first calculate the aproximate cell centre as the average of all
  // face centres
  vector cEst = vector::zero;
  scalar sumArea = 0;
  const labelList& faces = *this;
  FOR_ALL(faces, faceI)
  {
    scalar a = f[faces[faceI]].mag(p);
    cEst += f[faces[faceI]].centre(p)*a;
    sumArea += a;
  }
  cEst /= sumArea + VSMALL;
  // Calculate the centre by breaking the cell into pyramids and
  // volume-weighted averaging their centres
  vector sumVc = vector::zero;
  scalar sumV = 0;
  FOR_ALL(faces, faceI)
  {
    // calculate pyramid volume. If it is greater than zero, OK.
    // If not, the pyramid is inside-out. Create a face with the opposite
    // order and recalculate pyramid centre!
    scalar pyrVol = pyramidPointFaceRef(f[faces[faceI]], cEst).mag(p);
    vector pyrCentre = pyramidPointFaceRef(f[faces[faceI]], cEst).centre(p);
    // if pyramid inside-out because face points inwards invert
    // N.B. pyramid remains unchanged
    if (pyrVol < 0)
    {
      pyrVol = -pyrVol;
    }
    sumVc += pyrVol*pyrCentre;
    sumV += pyrVol;
  }
  return sumVc/(sumV + VSMALL);
}
mousse::scalar mousse::cell::mag
(
  const pointField& p,
  const faceUList& f
) const
{
  // When one wants to access the cell centre and magnitude, the
  // functionality on the mesh level should be used in preference to the
  // functions provided here. They do not rely to the functionality
  // implemented here, provide additional checking and are more efficient.
  // The cell::centre and cell::mag functions may be removed in the future.
  // WARNING! See cell::centre
  // first calculate the aproximate cell centre as the average of all
  // face centres
  vector cEst = vector::zero;
  scalar nCellFaces = 0;
  const labelList& faces = *this;
  FOR_ALL(faces, faceI)
  {
    cEst += f[faces[faceI]].centre(p);
    nCellFaces += 1;
  }
  cEst /= nCellFaces;
  // Calculate the magnitude by summing the mags of the pyramids
  scalar v = 0;
  FOR_ALL(faces, faceI)
  {
    v += ::mousse::mag(pyramidPointFaceRef(f[faces[faceI]], cEst).mag(p));
  }
  return v;
}
// Friend Operators 
bool mousse::operator==(const cell& a, const cell& b)
{
  // Trivial reject: faces are different size
  if (a.size() != b.size())
  {
    return false;
  }
  List<bool> fnd(a.size(), false);
  FOR_ALL(b, bI)
  {
    label curLabel = b[bI];
    bool found = false;
    FOR_ALL(a, aI)
    {
      if (a[aI] == curLabel)
      {
        found = true;
        fnd[aI] = true;
        break;
      }
    }
    if (!found)
    {
      return false;
    }
  }
  // check if all faces on a were marked
  bool result = true;
  FOR_ALL(fnd, aI)
  {
    result = (result && fnd[aI]);
  }
  return result;
}
