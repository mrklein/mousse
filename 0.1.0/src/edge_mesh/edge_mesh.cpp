// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_mesh.hpp"
#include "merge_points.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "list_ops.hpp"
#include "edge_map.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(edgeMesh, 0);
  defineRunTimeSelectionTable(edgeMesh, fileExtension);
  defineMemberFunctionSelectionTable(edgeMesh,write,fileExtension);
}
mousse::wordHashSet mousse::edgeMesh::readTypes()
{
  return wordHashSet(*fileExtensionConstructorTablePtr_);
}
mousse::wordHashSet mousse::edgeMesh::writeTypes()
{
  return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}
// Static Member Functions
bool mousse::edgeMesh::canReadType
(
  const word& ext,
  const bool verbose
)
{
  return checkSupport
  (
    readTypes(),
    ext,
    verbose,
    "reading"
 );
}
bool mousse::edgeMesh::canWriteType
(
  const word& ext,
  const bool verbose
)
{
  return checkSupport
  (
    writeTypes(),
    ext,
    verbose,
    "writing"
  );
}
bool mousse::edgeMesh::canRead
(
  const fileName& name,
  const bool verbose
)
{
  word ext = name.ext();
  if (ext == "gz")
  {
    ext = name.lessExt().ext();
  }
  return canReadType(ext, verbose);
}
// Private Member Functions 
void mousse::edgeMesh::calcPointEdges() const
{
  if (pointEdgesPtr_.valid())
  {
    FatalErrorIn("edgeMesh::calcPointEdges() const")
      << "pointEdges already calculated." << abort(FatalError);
  }
  pointEdgesPtr_.reset(new labelListList(points_.size()));
  labelListList& pointEdges = pointEdgesPtr_();
  invertManyToMany(pointEdges.size(), edges_, pointEdges);
}
// Constructors 
mousse::edgeMesh::edgeMesh()
:
  fileFormats::edgeMeshFormatsCore(),
  points_(0),
  edges_(0),
  pointEdgesPtr_(NULL)
{}
mousse::edgeMesh::edgeMesh
(
  const pointField& points,
  const edgeList& edges
)
:
  fileFormats::edgeMeshFormatsCore(),
  points_(points),
  edges_(edges),
  pointEdgesPtr_(NULL)
{}
mousse::edgeMesh::edgeMesh
(
  const Xfer<pointField>& pointLst,
  const Xfer<edgeList>& edgeLst
)
:
  fileFormats::edgeMeshFormatsCore(),
  points_(0),
  edges_(0),
  pointEdgesPtr_(NULL)
{
  points_.transfer(pointLst());
  edges_.transfer(edgeLst());
}
// Destructor 
mousse::edgeMesh::~edgeMesh()
{}
// Member Functions 
void mousse::edgeMesh::clear()
{
  points_.clear();
  edges_.clear();
  pointEdgesPtr_.clear();
}
void mousse::edgeMesh::reset
(
  const Xfer<pointField>& pointLst,
  const Xfer<edgeList>& edgeLst
)
{
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(pointLst))
  {
    points_.transfer(pointLst());
  }
  if (notNull(edgeLst))
  {
    edges_.transfer(edgeLst());
    // connectivity likely changed
    pointEdgesPtr_.clear();
  }
}
void mousse::edgeMesh::transfer(edgeMesh& mesh)
{
  points_.transfer(mesh.points_);
  edges_.transfer(mesh.edges_);
  pointEdgesPtr_ = mesh.pointEdgesPtr_;
}
mousse::Xfer<mousse::edgeMesh> mousse::edgeMesh::xfer()
{
  return xferMove(*this);
}
mousse::label mousse::edgeMesh::regions(labelList& edgeRegion) const
{
  edgeRegion.setSize(edges_.size());
  edgeRegion = -1;
  label startEdgeI = 0;
  label currentRegion = 0;
  while (true)
  {
    while (startEdgeI < edges_.size() && edgeRegion[startEdgeI] != -1)
    {
      startEdgeI++;
    }
    if (startEdgeI == edges_.size())
    {
      break;
    }
    // Found edge that has not yet been assigned a region.
    // Mark connected region with currentRegion starting at startEdgeI.
    edgeRegion[startEdgeI] = currentRegion;
    labelList edgesToVisit(1, startEdgeI);
    while (edgesToVisit.size())
    {
      // neighbours of current edgesToVisit
      DynamicList<label> newEdgesToVisit(edgesToVisit.size());
      // Mark all point connected edges with current region.
      forAll(edgesToVisit, i)
      {
        label edgeI = edgesToVisit[i];
        // Mark connected edges
        const edge& e = edges_[edgeI];
        forAll(e, fp)
        {
          const labelList& pEdges = pointEdges()[e[fp]];
          forAll(pEdges, pEdgeI)
          {
            label nbrEdgeI = pEdges[pEdgeI];
            if (edgeRegion[nbrEdgeI] == -1)
            {
              edgeRegion[nbrEdgeI] = currentRegion;
              newEdgesToVisit.append(nbrEdgeI);
            }
          }
        }
      }
      edgesToVisit.transfer(newEdgesToVisit);
    }
    currentRegion++;
  }
  return currentRegion;
}
void mousse::edgeMesh::scalePoints(const scalar scaleFactor)
{
  // avoid bad scaling
  if (scaleFactor > 0 && scaleFactor != 1.0)
  {
    points_ *= scaleFactor;
  }
}
void mousse::edgeMesh::mergePoints(const scalar mergeDist)
{
  pointField newPoints;
  labelList pointMap;
  bool hasMerged = mousse::mergePoints
  (
    points_,
    mergeDist,
    false,
    pointMap,
    newPoints,
    vector::zero
  );
  if (hasMerged)
  {
    pointEdgesPtr_.clear();
    points_.transfer(newPoints);
    // Renumber and make sure e[0] < e[1] (not really necessary)
    forAll(edges_, edgeI)
    {
      edge& e = edges_[edgeI];
      label p0 = pointMap[e[0]];
      label p1 = pointMap[e[1]];
      if (p0 < p1)
      {
        e[0] = p0;
        e[1] = p1;
      }
      else
      {
        e[0] = p1;
        e[1] = p0;
      }
    }
    // Compact using a hashtable and commutative hash of edge.
    EdgeMap<label> edgeToLabel(2*edges_.size());
    label newEdgeI = 0;
    forAll(edges_, edgeI)
    {
      const edge& e = edges_[edgeI];
      if (e[0] != e[1])
      {
        if (edgeToLabel.insert(e, newEdgeI))
        {
          newEdgeI++;
        }
      }
    }
    edges_.setSize(newEdgeI);
    forAllConstIter(EdgeMap<label>, edgeToLabel, iter)
    {
      edges_[iter()] = iter.key();
    }
  }
}
void mousse::edgeMesh::mergeEdges()
{
  EdgeMap<label> existingEdges(2*edges_.size());
  label curEdgeI = 0;
  forAll(edges_, edgeI)
  {
    const edge& e = edges_[edgeI];
    if (existingEdges.insert(e, curEdgeI))
    {
      curEdgeI++;
    }
  }
  if (debug)
  {
    Info<< "Merging duplicate edges: "
      << edges_.size() - existingEdges.size()
      << " edges will be deleted." << endl;
  }
  edges_.setSize(existingEdges.size());
  forAllConstIter(EdgeMap<label>, existingEdges, iter)
  {
    edges_[iter()] = iter.key();
  }
}
