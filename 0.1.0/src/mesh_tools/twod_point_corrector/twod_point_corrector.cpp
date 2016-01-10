// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "twod_point_corrector.hpp"
#include "poly_mesh.hpp"
#include "wedge_poly_patch.hpp"
#include "empty_poly_patch.hpp"
#include "sub_field.hpp"
#include "mesh_tools.hpp"
#include "demand_driven_data.hpp"

// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(twoDPointCorrector, 0);
}
const mousse::scalar mousse::twoDPointCorrector::edgeOrthogonalityTol = 1.0 - 1e-4;
// Private Member Functions
void mousse::twoDPointCorrector::calcAddressing() const
{
  // Find geometry normal
  planeNormalPtr_ = new vector(0, 0, 0);
  vector& pn = *planeNormalPtr_;
  // Algorithm:
  // Attempt to find wedge patch and work out the normal from it.
  // If not found, find an empty patch with faces in it and use the
  // normal of the first face.  If neither is found, declare an
  // error.
  // Try and find a wedge patch
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    if (isA<wedgePolyPatch>(patches[patchI]))
    {
      isWedge_ = true;
      const wedgePolyPatch& wp =
        refCast<const wedgePolyPatch>(patches[patchI]);
      pn = wp.centreNormal();
      wedgeAxis_ = wp.axis();
      wedgeAngle_ = mag(acos(wp.cosAngle()));
      if (polyMesh::debug)
      {
        Pout<< "Found normal from wedge patch " << patchI;
      }
      break;
    }
  }
  // Try to find an empty patch with faces
  if (!isWedge_)
  {
    FOR_ALL(patches, patchI)
    {
      if (isA<emptyPolyPatch>(patches[patchI]) && patches[patchI].size())
      {
        pn = patches[patchI].faceAreas()[0];
        if (polyMesh::debug)
        {
          Pout<< "Found normal from empty patch " << patchI;
        }
        break;
      }
    }
  }
  if (mag(pn) < VSMALL)
  {
    FATAL_ERROR_IN("twoDPointCorrector::calcAddressing()")
      << "Cannot determine normal vector from patches."
      << abort(FatalError);
  }
  else
  {
    pn /= mag(pn);
  }
  if (polyMesh::debug)
  {
    Pout<< " twoDPointCorrector normal: " << pn << endl;
  }
  // Select edges to be included in check.
  normalEdgeIndicesPtr_ = new labelList(mesh_.nEdges());
  labelList& neIndices = *normalEdgeIndicesPtr_;
  const edgeList& meshEdges = mesh_.edges();
  const pointField& meshPoints = mesh_.points();
  label nNormalEdges = 0;
  FOR_ALL(meshEdges, edgeI)
  {
    const edge& e = meshEdges[edgeI];
    vector edgeVector = e.vec(meshPoints)/(e.mag(meshPoints) + VSMALL);
    if (mag(edgeVector & pn) > edgeOrthogonalityTol)
    {
      // this edge is normal to the plane. Add it to the list
      neIndices[nNormalEdges++] = edgeI;
    }
  }
  neIndices.setSize(nNormalEdges);
  // Construction check: number of points in a read 2-D or wedge geometry
  // should be odd and the number of edges normal to the plane should be
  // exactly half the number of points
  if (!isWedge_)
  {
    if (meshPoints.size() % 2 != 0)
    {
      WARNING_IN("twoDPointCorrector::calcAddressing()")
        << "the number of vertices in the geometry "
        << "is odd - this should not be the case for a 2-D case. "
        << "Please check the geometry."
        << endl;
    }
    if (2*nNormalEdges != meshPoints.size())
    {
      WARNING_IN("twoDPointCorrector::calcAddressing()")
        << "The number of points in the mesh is "
        << "not equal to twice the number of edges normal to the plane "
        << "- this may be OK only for wedge geometries.\n"
        << "    Please check the geometry or adjust "
        << "the orthogonality tolerance.\n" << endl
        << "Number of normal edges: " << nNormalEdges
        << " number of points: " << meshPoints.size()
        << endl;
    }
  }
}
void mousse::twoDPointCorrector::clearAddressing() const
{
  deleteDemandDrivenData(planeNormalPtr_);
  deleteDemandDrivenData(normalEdgeIndicesPtr_);
}
void mousse::twoDPointCorrector::snapToWedge
(
  const vector& n,
  const point& A,
  point& p
) const
{
  scalar ADash = mag(A - wedgeAxis_*(wedgeAxis_ & A));
  vector pDash = ADash*tan(wedgeAngle_)*planeNormal();
  p = A + sign(n & p)*pDash;
}
// Constructors
mousse::twoDPointCorrector::twoDPointCorrector(const polyMesh& mesh)
:
  MeshObject<polyMesh, mousse::UpdateableMeshObject, twoDPointCorrector>(mesh),
  required_(mesh_.nGeometricD() == 2),
  planeNormalPtr_(NULL),
  normalEdgeIndicesPtr_(NULL),
  isWedge_(false),
  wedgeAxis_(vector::zero),
  wedgeAngle_(0.0)
{}

// Destructor
mousse::twoDPointCorrector::~twoDPointCorrector()
{
  clearAddressing();
}

// Member Functions
mousse::direction mousse::twoDPointCorrector::normalDir() const
{
  const vector& pn = planeNormal();
  if (mag(pn.x()) >= edgeOrthogonalityTol)
  {
    return vector::X;
  }
  else if (mag(pn.y()) >= edgeOrthogonalityTol)
  {
    return vector::Y;
  }
  else if (mag(pn.z()) >= edgeOrthogonalityTol)
  {
    return vector::Z;
  }
  else
  {
    FATAL_ERROR_IN("direction twoDPointCorrector::normalDir() const")
      << "Plane normal not aligned with the coordinate system" << nl
      << "    pn = " << pn
      << abort(FatalError);
    return vector::Z;
  }
}
const mousse::vector& mousse::twoDPointCorrector::planeNormal() const
{
  if (!planeNormalPtr_)
  {
    calcAddressing();
  }
  return *planeNormalPtr_;
}
const mousse::labelList& mousse::twoDPointCorrector::normalEdgeIndices() const
{
  if (!normalEdgeIndicesPtr_)
  {
    calcAddressing();
  }
  return *normalEdgeIndicesPtr_;
}
void mousse::twoDPointCorrector::correctPoints(pointField& p) const
{
  if (!required_) return;
  // Algorithm:
  // Loop through all edges. Calculate the average point position A for
  // the front and the back. Correct the position of point P (in two planes)
  // such that vectors AP and planeNormal are parallel
  // Get reference to edges
  const edgeList&  meshEdges = mesh_.edges();
  const labelList& neIndices = normalEdgeIndices();
  const vector& pn = planeNormal();
  FOR_ALL(neIndices, edgeI)
  {
    point& pStart = p[meshEdges[neIndices[edgeI]].start()];
    point& pEnd = p[meshEdges[neIndices[edgeI]].end()];
    // calculate average point position
    point A = 0.5*(pStart + pEnd);
    meshTools::constrainToMeshCentre(mesh_, A);
    if (isWedge_)
    {
      snapToWedge(pn, A, pStart);
      snapToWedge(pn, A, pEnd);
    }
    else
    {
      // correct point locations
      pStart = A + pn*(pn & (pStart - A));
      pEnd = A + pn*(pn & (pEnd - A));
    }
  }
}
void mousse::twoDPointCorrector::correctDisplacement
(
  const pointField& p,
  vectorField& disp
) const
{
  if (!required_) return;
  // Algorithm:
  // Loop through all edges. Calculate the average point position A for
  // the front and the back. Correct the position of point P (in two planes)
  // such that vectors AP and planeNormal are parallel
  // Get reference to edges
  const edgeList&  meshEdges = mesh_.edges();
  const labelList& neIndices = normalEdgeIndices();
  const vector& pn = planeNormal();
  FOR_ALL(neIndices, edgeI)
  {
    const edge& e = meshEdges[neIndices[edgeI]];
    label startPointI = e.start();
    point pStart = p[startPointI] + disp[startPointI];
    label endPointI = e.end();
    point pEnd = p[endPointI] + disp[endPointI];
    // calculate average point position
    point A = 0.5*(pStart + pEnd);
    meshTools::constrainToMeshCentre(mesh_, A);
    if (isWedge_)
    {
      snapToWedge(pn, A, pStart);
      snapToWedge(pn, A, pEnd);
      disp[startPointI] = pStart - p[startPointI];
      disp[endPointI] = pEnd - p[endPointI];
    }
    else
    {
      // correct point locations
      disp[startPointI] = (A + pn*(pn & (pStart - A))) - p[startPointI];
      disp[endPointI] = (A + pn*(pn & (pEnd - A))) - p[endPointI];
    }
  }
}
void mousse::twoDPointCorrector::updateMesh(const mapPolyMesh&)
{
  clearAddressing();
}
bool mousse::twoDPointCorrector::movePoints()
{
  return true;
}
