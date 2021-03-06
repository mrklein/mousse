// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_edge_mesh.hpp"
#include "surface_features.hpp"
#include "tri_surface.hpp"
#include "random.hpp"
#include "time.hpp"
#include "obj_stream.hpp"
#include "dynamic_field.hpp"
#include "edge_mesh_formats_core.hpp"
#include "iomanip.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(extendedEdgeMesh, 0);

template<>
const char* mousse::NamedEnum
<
  mousse::extendedEdgeMesh::pointStatus,
  4
>::names[] =
{
  "convex",
  "concave",
  "mixed",
  "nonFeature"
};

template<>
const char* mousse::NamedEnum
<
  mousse::extendedEdgeMesh::edgeStatus,
  6
>::names[] =
{
  "external",
  "internal",
  "flat",
  "open",
  "multiple",
  "none"
};

template<>
const char* mousse::NamedEnum
<
  mousse::extendedEdgeMesh::sideVolumeType,
  4
>::names[] =
{
  "inside",
  "outside",
  "both",
  "neither"
};

}

const mousse::NamedEnum<mousse::extendedEdgeMesh::pointStatus, 4>
  mousse::extendedEdgeMesh::pointStatusNames_;
const mousse::NamedEnum<mousse::extendedEdgeMesh::edgeStatus, 6>
  mousse::extendedEdgeMesh::edgeStatusNames_;
const mousse::NamedEnum<mousse::extendedEdgeMesh::sideVolumeType, 4>
  mousse::extendedEdgeMesh::sideVolumeTypeNames_;
mousse::scalar mousse::extendedEdgeMesh::cosNormalAngleTol_ =
  mousse::cos(degToRad(0.1));
mousse::label mousse::extendedEdgeMesh::convexStart_ = 0;
mousse::label mousse::extendedEdgeMesh::externalStart_ = 0;
mousse::label mousse::extendedEdgeMesh::nPointTypes = 4;
mousse::label mousse::extendedEdgeMesh::nEdgeTypes = 5;


mousse::wordHashSet mousse::extendedEdgeMesh::readTypes()
{
  return {*fileExtensionConstructorTablePtr_};
}


mousse::wordHashSet mousse::extendedEdgeMesh::writeTypes()
{
  return {*writefileExtensionMemberFunctionTablePtr_};
}


// Static Member Functions
bool mousse::extendedEdgeMesh::canReadType
(
  const word& ext,
  const bool verbose
)
{
  return edgeMeshFormatsCore::checkSupport
  (
    readTypes(),
    ext,
    verbose,
    "reading"
 );
}


bool mousse::extendedEdgeMesh::canWriteType
(
  const word& ext,
  const bool verbose
)
{
  return edgeMeshFormatsCore::checkSupport
  (
    writeTypes(),
    ext,
    verbose,
    "writing"
  );
}


bool mousse::extendedEdgeMesh::canRead
(
  const fileName& name,
  const bool verbose
)
{
  word ext = name.ext();
  if (ext == "gz") {
    ext = name.lessExt().ext();
  }
  return canReadType(ext, verbose);
}


// Private Member Functions 
mousse::extendedEdgeMesh::pointStatus
mousse::extendedEdgeMesh::classifyFeaturePoint
(
  label ptI
) const
{
  labelList ptEds{pointEdges()[ptI]};
  label nPtEds = ptEds.size();
  label nExternal = 0;
  label nInternal = 0;
  if (nPtEds == 0) {
    // There are no edges attached to the point, this is a problem
    return NONFEATURE;
  }
  FOR_ALL(ptEds, i) {
    edgeStatus edStat = getEdgeStatus(ptEds[i]);
    if (edStat == EXTERNAL) {
      nExternal++;
    } else if (edStat == INTERNAL) {
      nInternal++;
    }
  }
  if (nExternal == nPtEds) {
    return CONVEX;
  } else if (nInternal == nPtEds) {
    return CONCAVE;
  } else {
    return MIXED;
  }
}


mousse::extendedEdgeMesh::edgeStatus
mousse::extendedEdgeMesh::classifyEdge
(
  const List<vector>& norms,
  const labelList& edNorms,
  const vector& fC0tofC1
)
{
  label nEdNorms = edNorms.size();
  if (nEdNorms == 1) {
    return OPEN;
  } else if (nEdNorms == 2) {
    const vector& n0{norms[edNorms[0]]};
    const vector& n1{norms[edNorms[1]]};
    if ((n0 & n1) > cosNormalAngleTol_) {
      return FLAT;
    } else if ((fC0tofC1 & n0) > 0.0) {
      return INTERNAL;
    } else {
      return EXTERNAL;
    }
  } else if (nEdNorms > 2) {
    return MULTIPLE;
  } else {
    // There is a problem - the edge has no normals
    return NONE;
  }
}


// Constructors 
mousse::extendedEdgeMesh::extendedEdgeMesh()
:
  edgeMesh{pointField(0), edgeList(0)},
  concaveStart_{0},
  mixedStart_{0},
  nonFeatureStart_{0},
  internalStart_{0},
  flatStart_{0},
  openStart_{0},
  multipleStart_{0},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const extendedEdgeMesh& fem
)
:
  edgeMesh{fem},
  concaveStart_{fem.concaveStart()},
  mixedStart_{fem.mixedStart()},
  nonFeatureStart_{fem.nonFeatureStart()},
  internalStart_{fem.internalStart()},
  flatStart_{fem.flatStart()},
  openStart_{fem.openStart()},
  multipleStart_{fem.multipleStart()},
  normals_{fem.normals()},
  normalVolumeTypes_{fem.normalVolumeTypes()},
  edgeDirections_{fem.edgeDirections()},
  normalDirections_{fem.normalDirections()},
  edgeNormals_{fem.edgeNormals()},
  featurePointNormals_{fem.featurePointNormals()},
  featurePointEdges_{fem.featurePointEdges()},
  regionEdges_{fem.regionEdges()},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{}


mousse::extendedEdgeMesh::extendedEdgeMesh(Istream& is)
{
  is >> *this;
}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const Xfer<pointField>& pointLst,
  const Xfer<edgeList>& edgeLst
)
:
  edgeMesh{pointLst, edgeLst},
  concaveStart_{0},
  mixedStart_{0},
  nonFeatureStart_{0},
  internalStart_{0},
  flatStart_{0},
  openStart_{0},
  multipleStart_{0},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const surfaceFeatures& sFeat,
  const boolList& surfBaffleRegions
)
:
  edgeMesh{pointField(0), edgeList(0)},
  concaveStart_{-1},
  mixedStart_{-1},
  nonFeatureStart_{-1},
  internalStart_{-1},
  flatStart_{-1},
  openStart_{-1},
  multipleStart_{-1},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{
  // Extract and reorder the data from surfaceFeatures
  const triSurface& surf = sFeat.surface();
  const labelList& featureEdges = sFeat.featureEdges();
  const labelList& featurePoints = sFeat.featurePoints();
  // Get a labelList of all the featureEdges that are region edges
  const labelList regionFeatureEdges{identity(sFeat.nRegionEdges())};
  sortPointsAndEdges
  (
    surf,
    featureEdges,
    regionFeatureEdges,
    featurePoints
  );
  const labelListList& edgeFaces = surf.edgeFaces();
  normalVolumeTypes_.setSize(normals_.size());
  // Noting when the normal of a face has been used so not to duplicate
  labelList faceMap{surf.size(), -1};
  label nAdded = 0;
  FOR_ALL(featureEdges, i) {
    label sFEI = featureEdges[i];
    // Pick up the faces adjacent to the feature edge
    const labelList& eFaces = edgeFaces[sFEI];
    FOR_ALL(eFaces, j) {
      label eFI = eFaces[j];
      // Check to see if the points have been already used
      if (faceMap[eFI] == -1) {
        normalVolumeTypes_[nAdded++] =
          surfBaffleRegions[surf[eFI].region()] ? BOTH : INSIDE;
        faceMap[eFI] = nAdded - 1;
      }
    }
  }
}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const PrimitivePatch<face, List, pointField, point>& surf,
  const labelList& featureEdges,
  const labelList& regionFeatureEdges,
  const labelList& featurePoints
)
:
  edgeMesh{pointField(0), edgeList(0)},
  concaveStart_{-1},
  mixedStart_{-1},
  nonFeatureStart_{-1},
  internalStart_{-1},
  flatStart_{-1},
  openStart_{-1},
  multipleStart_{-1},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{
  sortPointsAndEdges
  (
    surf,
    featureEdges,
    regionFeatureEdges,
    featurePoints
  );
}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const pointField& pts,
  const edgeList& eds,
  label concaveStart,
  label mixedStart,
  label nonFeatureStart,
  label internalStart,
  label flatStart,
  label openStart,
  label multipleStart,
  const vectorField& normals,
  const List<sideVolumeType>& normalVolumeTypes,
  const vectorField& edgeDirections,
  const labelListList& normalDirections,
  const labelListList& edgeNormals,
  const labelListList& featurePointNormals,
  const labelListList& featurePointEdges,
  const labelList& regionEdges
)
:
  edgeMesh{pts, eds},
  concaveStart_{concaveStart},
  mixedStart_{mixedStart},
  nonFeatureStart_{nonFeatureStart},
  internalStart_{internalStart},
  flatStart_{flatStart},
  openStart_{openStart},
  multipleStart_{multipleStart},
  normals_{normals},
  normalVolumeTypes_{normalVolumeTypes},
  edgeDirections_{edgeDirections},
  normalDirections_{normalDirections},
  edgeNormals_{edgeNormals},
  featurePointNormals_{featurePointNormals},
  featurePointEdges_{featurePointEdges},
  regionEdges_{regionEdges},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{}


mousse::extendedEdgeMesh::extendedEdgeMesh
(
  const fileName& name,
  const word& ext
)
:
  edgeMesh{pointField(0), edgeList(0)},
  concaveStart_{0},
  mixedStart_{0},
  nonFeatureStart_{0},
  internalStart_{0},
  flatStart_{0},
  openStart_{0},
  multipleStart_{0},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{
  read(name, ext);
}


mousse::extendedEdgeMesh::extendedEdgeMesh(const fileName& name)
:
  edgeMesh{pointField(0), edgeList(0)},
  concaveStart_{0},
  mixedStart_{0},
  nonFeatureStart_{0},
  internalStart_{0},
  flatStart_{0},
  openStart_{0},
  multipleStart_{0},
  normals_{0},
  normalVolumeTypes_{0},
  edgeDirections_{0},
  normalDirections_{0},
  edgeNormals_{0},
  featurePointNormals_{0},
  featurePointEdges_{0},
  regionEdges_{0},
  pointTree_{},
  edgeTree_{},
  edgeTreesByType_{}
{
  read(name);
}


// Destructor 
mousse::extendedEdgeMesh::~extendedEdgeMesh()
{}


// Member Functions 
bool mousse::extendedEdgeMesh::read(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz") {
    fileName unzipName = name.lessExt();
    return read(unzipName, unzipName.ext());
  } else {
    return read(name, ext);
  }
}


// Read from file in given format
bool mousse::extendedEdgeMesh::read
(
  const fileName& name,
  const word& ext
)
{
  // read via selector mechanism
  transfer(New(name, ext)());
  return true;
}


void mousse::extendedEdgeMesh::nearestFeaturePoint
(
  const point& sample,
  scalar searchDistSqr,
  pointIndexHit& info
) const
{
  info = pointTree().findNearest
  (
    sample,
    searchDistSqr
  );
}


void mousse::extendedEdgeMesh::nearestFeatureEdge
(
  const point& sample,
  scalar searchDistSqr,
  pointIndexHit& info
) const
{
  info = edgeTree().findNearest
  (
    sample,
    searchDistSqr
  );
}


void mousse::extendedEdgeMesh::nearestFeatureEdge
(
  const pointField& samples,
  const scalarField& searchDistSqr,
  List<pointIndexHit>& info
) const
{
  info.setSize(samples.size());
  FOR_ALL(samples, i) {
    nearestFeatureEdge
    (
      samples[i],
      searchDistSqr[i],
      info[i]
    );
  }
}


void mousse::extendedEdgeMesh::nearestFeatureEdgeByType
(
  const point& sample,
  const scalarField& searchDistSqr,
  List<pointIndexHit>& info
) const
{
  const PtrList<indexedOctree<treeDataEdge>>& edgeTrees = edgeTreesByType();
  info.setSize(edgeTrees.size());
  labelList sliceStarts{edgeTrees.size()};
  sliceStarts[0] = externalStart_;
  sliceStarts[1] = internalStart_;
  sliceStarts[2] = flatStart_;
  sliceStarts[3] = openStart_;
  sliceStarts[4] = multipleStart_;
  FOR_ALL(edgeTrees, i) {
    info[i] = edgeTrees[i].findNearest
    (
      sample,
      searchDistSqr[i]
    );
    // The index returned by the indexedOctree is local to the slice of
    // edges it was supplied with, return the index to the value in the
    // complete edge list
    info[i].setIndex(info[i].index() + sliceStarts[i]);
  }
}


void mousse::extendedEdgeMesh::allNearestFeaturePoints
(
  const point& sample,
  scalar searchRadiusSqr,
  List<pointIndexHit>& info
) const
{
  // Pick up all the feature points that intersect the search sphere
  labelList elems = pointTree().findSphere
  (
    sample,
    searchRadiusSqr
  );
  DynamicList<pointIndexHit> dynPointHit{elems.size()};
  FOR_ALL(elems, elemI) {
    label index = elems[elemI];
    label ptI = pointTree().shapes().pointLabels()[index];
    const point& pt = points()[ptI];
    pointIndexHit nearHit(true, pt, index);
    dynPointHit.append(nearHit);
  }
  info.transfer(dynPointHit);
}


void mousse::extendedEdgeMesh::allNearestFeatureEdges
(
  const point& sample,
  const scalar searchRadiusSqr,
  List<pointIndexHit>& info
) const
{
  const PtrList<indexedOctree<treeDataEdge>>& edgeTrees = edgeTreesByType();
  info.setSize(edgeTrees.size());
  labelList sliceStarts{edgeTrees.size()};
  sliceStarts[0] = externalStart_;
  sliceStarts[1] = internalStart_;
  sliceStarts[2] = flatStart_;
  sliceStarts[3] = openStart_;
  sliceStarts[4] = multipleStart_;
  DynamicList<pointIndexHit> dynEdgeHit{edgeTrees.size()*3};
  // Loop over all the feature edge types
  FOR_ALL(edgeTrees, i) {
    // Pick up all the edges that intersect the search sphere
    labelList elems = edgeTrees[i].findSphere
    (
      sample,
      searchRadiusSqr
    );
    FOR_ALL(elems, elemI) {
      label index = elems[elemI];
      label edgeI = edgeTrees[i].shapes().edgeLabels()[index];
      const edge& e = edges()[edgeI];
      pointHit hitPoint = e.line(points()).nearestDist(sample);
      label hitIndex = index + sliceStarts[i];
      pointIndexHit nearHit{hitPoint.hit(), hitPoint.rawPoint(), hitIndex};
      dynEdgeHit.append(nearHit);
    }
  }
  info.transfer(dynEdgeHit);
}


const mousse::indexedOctree<mousse::treeDataPoint>&
mousse::extendedEdgeMesh::pointTree() const
{
  if (pointTree_.empty()) {
    Random rndGen{17301893};
    // Slightly extended bb. Slightly off-centred just so on symmetric
    // geometry there are less face/edge aligned items.
    treeBoundBox bb{treeBoundBox(points()).extend(rndGen, 1e-4)};
    bb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    bb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    const labelList featurePointLabels = identity(nonFeatureStart_);
    pointTree_.reset
    (
      new indexedOctree<treeDataPoint>
      {
        treeDataPoint{points(), featurePointLabels},
        bb,     // bb
        8,      // maxLevel
        10,     // leafsize
        3.0     // duplicity
      }
    );
  }
  return pointTree_();
}


const mousse::indexedOctree<mousse::treeDataEdge>&
mousse::extendedEdgeMesh::edgeTree() const
{
  if (edgeTree_.empty()) {
    Random rndGen{17301893};
    // Slightly extended bb. Slightly off-centred just so on symmetric
    // geometry there are less face/edge aligned items.
    treeBoundBox bb{treeBoundBox(points()).extend(rndGen, 1e-4)};
    bb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    bb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    labelList allEdges{identity(edges().size())};
    edgeTree_.reset
    (
      new indexedOctree<treeDataEdge>
      {
        treeDataEdge
        {
          false,          // cachebb
          edges(),        // edges
          points(),       // points
          allEdges        // selected edges
        },
        bb,     // bb
        8,      // maxLevel
        10,     // leafsize
        3.0     // duplicity
      }
    );
  }
  return edgeTree_();
}


const mousse::PtrList<mousse::indexedOctree<mousse::treeDataEdge> >&
mousse::extendedEdgeMesh::edgeTreesByType() const
{
  if (edgeTreesByType_.size() == 0) {
    edgeTreesByType_.setSize(nEdgeTypes);
    Random rndGen{872141};
    // Slightly extended bb. Slightly off-centred just so on symmetric
    // geometry there are less face/edge aligned items.
    treeBoundBox bb{treeBoundBox(points()).extend(rndGen, 1e-4)};
    bb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    bb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    labelListList sliceEdges{nEdgeTypes};
    // External edges
    sliceEdges[0] =
      identity(internalStart_ - externalStart_) + externalStart_;
    // Internal edges
    sliceEdges[1] = identity(flatStart_ - internalStart_) + internalStart_;
    // Flat edges
    sliceEdges[2] = identity(openStart_ - flatStart_) + flatStart_;
    // Open edges
    sliceEdges[3] = identity(multipleStart_ - openStart_) + openStart_;
    // Multiple edges
    sliceEdges[4] =
      identity(edges().size() - multipleStart_) + multipleStart_;
    FOR_ALL(edgeTreesByType_, i) {
      edgeTreesByType_.set
      (
        i,
        new indexedOctree<treeDataEdge>
        {
          treeDataEdge
          {
            false,          // cachebb
            edges(),        // edges
            points(),       // points
            sliceEdges[i]   // selected edges
          },
          bb,     // bb
          8,      // maxLevel
          10,     // leafsize
          3.0     // duplicity
        }
      );
    }
  }
  return edgeTreesByType_;
}


void mousse::extendedEdgeMesh::transfer(extendedEdgeMesh& mesh)
{
  edgeMesh::transfer(mesh);
  concaveStart_ = mesh.concaveStart_;
  mixedStart_ = mesh.mixedStart_;
  nonFeatureStart_ = mesh.nonFeatureStart_;
  internalStart_ = mesh.internalStart_;
  flatStart_ = mesh.flatStart_;
  openStart_ = mesh.openStart_;
  multipleStart_ = mesh.multipleStart_;
  normals_.transfer(mesh.normals_);
  normalVolumeTypes_.transfer(mesh.normalVolumeTypes_);
  edgeDirections_.transfer(mesh.edgeDirections_);
  normalDirections_.transfer(mesh.normalDirections_);
  edgeNormals_.transfer(mesh.edgeNormals_);
  featurePointNormals_.transfer(mesh.featurePointNormals_);
  featurePointEdges_.transfer(mesh.featurePointEdges_);
  regionEdges_.transfer(mesh.regionEdges_);
  pointTree_ = mesh.pointTree_;
  edgeTree_ = mesh.edgeTree_;
  edgeTreesByType_.transfer(mesh.edgeTreesByType_);
}


mousse::Xfer<mousse::extendedEdgeMesh> mousse::extendedEdgeMesh::xfer()
{
  return xferMoveTo<extendedEdgeMesh, extendedEdgeMesh>(*this);
}


void mousse::extendedEdgeMesh::clear()
{
  edgeMesh::clear();
  concaveStart_ = 0;
  mixedStart_ = 0;
  nonFeatureStart_ = 0;
  internalStart_ = 0;
  flatStart_ = 0;
  openStart_ = 0;
  multipleStart_ = 0;
  normals_.clear();
  normalVolumeTypes_.clear();
  edgeDirections_.clear();
  normalDirections_.clear();
  edgeNormals_.clear();
  featurePointNormals_.clear();
  featurePointEdges_.clear();
  regionEdges_.clear();
  pointTree_.clear();
  edgeTree_.clear();
  edgeTreesByType_.clear();
}


void mousse::extendedEdgeMesh::add(const extendedEdgeMesh& fem)
{
  // Points
  // ~~~~~~
  // From current points into combined points
  labelList reversePointMap{points().size()};
  labelList reverseFemPointMap{fem.points().size()};
  label newPointI = 0;
  for (label i = 0; i < concaveStart(); i++) {
    reversePointMap[i] = newPointI++;
  }
  for (label i = 0; i < fem.concaveStart(); i++) {
    reverseFemPointMap[i] = newPointI++;
  }
  // Concave
  label newConcaveStart = newPointI;
  for (label i = concaveStart(); i < mixedStart(); i++) {
    reversePointMap[i] = newPointI++;
  }
  for (label i = fem.concaveStart(); i < fem.mixedStart(); i++) {
    reverseFemPointMap[i] = newPointI++;
  }
  // Mixed
  label newMixedStart = newPointI;
  for (label i = mixedStart(); i < nonFeatureStart(); i++) {
    reversePointMap[i] = newPointI++;
  }
  for (label i = fem.mixedStart(); i < fem.nonFeatureStart(); i++) {
    reverseFemPointMap[i] = newPointI++;
  }
  // Non-feature
  label newNonFeatureStart = newPointI;
  for (label i = nonFeatureStart(); i < points().size(); i++) {
    reversePointMap[i] = newPointI++;
  }
  for (label i = fem.nonFeatureStart(); i < fem.points().size(); i++) {
    reverseFemPointMap[i] = newPointI++;
  }
  pointField newPoints{newPointI};
  newPoints.rmap(points(), reversePointMap);
  newPoints.rmap(fem.points(), reverseFemPointMap);
  // Edges
  // ~~~~~
  // From current edges into combined edges
  labelList reverseEdgeMap{edges().size()};
  labelList reverseFemEdgeMap{fem.edges().size()};
  // External
  label newEdgeI = 0;
  for (label i = 0; i < internalStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  for (label i = 0; i < fem.internalStart(); i++) {
    reverseFemEdgeMap[i] = newEdgeI++;
  }
  // Internal
  label newInternalStart = newEdgeI;
  for (label i = internalStart(); i < flatStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  for (label i = fem.internalStart(); i < fem.flatStart(); i++) {
    reverseFemEdgeMap[i] = newEdgeI++;
  }
  // Flat
  label newFlatStart = newEdgeI;
  for (label i = flatStart(); i < openStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  for (label i = fem.flatStart(); i < fem.openStart(); i++) {
    reverseFemEdgeMap[i] = newEdgeI++;
  }
  // Open
  label newOpenStart = newEdgeI;
  for (label i = openStart(); i < multipleStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  for (label i = fem.openStart(); i < fem.multipleStart(); i++) {
    reverseFemEdgeMap[i] = newEdgeI++;
  }
  // Multiple
  label newMultipleStart = newEdgeI;
  for (label i = multipleStart(); i < edges().size(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  for (label i = fem.multipleStart(); i < fem.edges().size(); i++) {
    reverseFemEdgeMap[i] = newEdgeI++;
  }
  edgeList newEdges{newEdgeI};
  FOR_ALL(edges(), i) {
    const edge& e = edges()[i];
    newEdges[reverseEdgeMap[i]] = edge{reversePointMap[e[0]],
                                       reversePointMap[e[1]]};
  }
  FOR_ALL(fem.edges(), i) {
    const edge& e = fem.edges()[i];
    newEdges[reverseFemEdgeMap[i]] = edge{reverseFemPointMap[e[0]],
                                          reverseFemPointMap[e[1]]};
  }
  pointField newEdgeDirections{newEdgeI};
  newEdgeDirections.rmap(edgeDirections(), reverseEdgeMap);
  newEdgeDirections.rmap(fem.edgeDirections(), reverseFemEdgeMap);
  // Normals
  // ~~~~~~~
  // Combine normals
  DynamicField<point> newNormals{normals().size()+fem.normals().size()};
  newNormals.append(normals());
  newNormals.append(fem.normals());
  // Combine and re-index into newNormals
  labelListList newEdgeNormals{edgeNormals().size()+fem.edgeNormals().size()};
  UIndirectList<labelList>(newEdgeNormals, reverseEdgeMap) =
    edgeNormals();
  UIndirectList<labelList>(newEdgeNormals, reverseFemEdgeMap) =
    fem.edgeNormals();
  FOR_ALL(reverseFemEdgeMap, i) {
    label mapI = reverseFemEdgeMap[i];
    labelList& en = newEdgeNormals[mapI];
    FOR_ALL(en, j) {
      en[j] += normals().size();
    }
  }
  // Combine and re-index into newFeaturePointNormals
  labelListList newFeaturePointNormals
  (
    featurePointNormals().size() + fem.featurePointNormals().size()
  );
  // Note: featurePointNormals only go up to nonFeatureStart
  UIndirectList<labelList>
  (
    newFeaturePointNormals,
    SubList<label>(reversePointMap, featurePointNormals().size())
  ) = featurePointNormals();
  UIndirectList<labelList>
  (
    newFeaturePointNormals,
    SubList<label>(reverseFemPointMap, fem.featurePointNormals().size())
  ) = fem.featurePointNormals();
  FOR_ALL(fem.featurePointNormals(), i) {
    label mapI = reverseFemPointMap[i];
    labelList& fn = newFeaturePointNormals[mapI];
    FOR_ALL(fn, j) {
      fn[j] += normals().size();
    }
  }
  // Combine regionEdges
  DynamicList<label> newRegionEdges
  (
    regionEdges().size() + fem.regionEdges().size()
  );
  FOR_ALL(regionEdges(), i) {
    newRegionEdges.append(reverseEdgeMap[regionEdges()[i]]);
  }
  FOR_ALL(fem.regionEdges(), i) {
    newRegionEdges.append(reverseFemEdgeMap[fem.regionEdges()[i]]);
  }
  // Assign
  // ~~~~~~
  // Transfer
  concaveStart_ = newConcaveStart;
  mixedStart_ = newMixedStart;
  nonFeatureStart_ = newNonFeatureStart;
  // Reset points and edges
  reset(xferMove(newPoints), newEdges.xfer());
  // Transfer
  internalStart_ = newInternalStart;
  flatStart_ = newFlatStart;
  openStart_ = newOpenStart;
  multipleStart_ = newMultipleStart;
  edgeDirections_.transfer(newEdgeDirections);
  normals_.transfer(newNormals);
  edgeNormals_.transfer(newEdgeNormals);
  featurePointNormals_.transfer(newFeaturePointNormals);
  regionEdges_.transfer(newRegionEdges);
  pointTree_.clear();
  edgeTree_.clear();
  edgeTreesByType_.clear();
}


void mousse::extendedEdgeMesh::flipNormals()
{
  // Points
  // ~~~~~~
  // From current points into new points
  labelList reversePointMap{identity(points().size())};
  // Flip convex and concave points
  label newPointI = 0;
  // Concave points become convex
  for (label i = concaveStart(); i < mixedStart(); i++) {
    reversePointMap[i] = newPointI++;
  }
  // Convex points become concave
  label newConcaveStart = newPointI;
  for (label i = 0; i < concaveStart(); i++) {
    reversePointMap[i] = newPointI++;
  }
  // Edges
  // ~~~~~~
  // From current edges into new edges
  labelList reverseEdgeMap{identity(edges().size())};
  // Flip external and internal edges
  label newEdgeI = 0;
  // Internal become external
  for (label i = internalStart(); i < flatStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  // External become internal
  label newInternalStart = newEdgeI;
  for (label i = 0; i < internalStart(); i++) {
    reverseEdgeMap[i] = newEdgeI++;
  }
  pointField newPoints{points().size()};
  newPoints.rmap(points(), reversePointMap);
  edgeList newEdges{edges().size()};
  FOR_ALL(edges(), i) {
    const edge& e = edges()[i];
    newEdges[reverseEdgeMap[i]] = edge{reversePointMap[e[0]],
                                       reversePointMap[e[1]]};
  }
  // Normals are flipped
  // ~~~~~~~~~~~~~~~~~~~
  pointField newEdgeDirections{edges().size()};
  newEdgeDirections.rmap(-1.0*edgeDirections(), reverseEdgeMap);
  pointField newNormals{-1.0*normals()};
  labelListList newEdgeNormals{edgeNormals().size()};
  UIndirectList<labelList>{newEdgeNormals, reverseEdgeMap} = edgeNormals();
  labelListList newFeaturePointNormals{featurePointNormals().size()};
  // Note: featurePointNormals only go up to nonFeatureStart
  UIndirectList<labelList>
  (
    newFeaturePointNormals,
    SubList<label>(reversePointMap, featurePointNormals().size())
  ) = featurePointNormals();
  labelList newRegionEdges{regionEdges().size()};
  FOR_ALL(regionEdges(), i) {
    newRegionEdges[i] = reverseEdgeMap[regionEdges()[i]];
  }
  // Transfer
  concaveStart_ = newConcaveStart;
  // Reset points and edges
  reset(xferMove(newPoints), newEdges.xfer());
  // Transfer
  internalStart_ = newInternalStart;
  edgeDirections_.transfer(newEdgeDirections);
  normals_.transfer(newNormals);
  edgeNormals_.transfer(newEdgeNormals);
  featurePointNormals_.transfer(newFeaturePointNormals);
  regionEdges_.transfer(newRegionEdges);
  pointTree_.clear();
  edgeTree_.clear();
  edgeTreesByType_.clear();
}


void mousse::extendedEdgeMesh::writeObj
(
  const fileName& prefix
) const
{
  Info << nl << "Writing extendedEdgeMesh components to " << prefix << endl;
  edgeMesh::write(prefix + "_edgeMesh.obj");
  OBJstream convexFtPtStr{prefix + "_convexFeaturePts.obj"};
  Info << "Writing convex feature points to " << convexFtPtStr.name() << endl;
  for(label i = 0; i < concaveStart_; i++) {
    convexFtPtStr.write(points()[i]);
  }
  OBJstream concaveFtPtStr{prefix + "_concaveFeaturePts.obj"};
  Info << "Writing concave feature points to "
    << concaveFtPtStr.name() << endl;
  for(label i = concaveStart_; i < mixedStart_; i++) {
    convexFtPtStr.write(points()[i]);
  }
  OBJstream mixedFtPtStr{prefix + "_mixedFeaturePts.obj"};
  Info << "Writing mixed feature points to " << mixedFtPtStr.name() << endl;
  for(label i = mixedStart_; i < nonFeatureStart_; i++) {
    mixedFtPtStr.write(points()[i]);
  }
  OBJstream mixedFtPtStructureStr{prefix + "_mixedFeaturePtsStructure.obj"};
  Info << "Writing mixed feature point structure to "
    << mixedFtPtStructureStr.name() << endl;
  for(label i = mixedStart_; i < nonFeatureStart_; i++) {
    const labelList& ptEds = pointEdges()[i];
    FOR_ALL(ptEds, j) {
      const edge& e = edges()[ptEds[j]];
      mixedFtPtStructureStr.write
      (
        linePointRef(points()[e[0]],
        points()[e[1]])
      );
    }
  }
  OBJstream externalStr{prefix + "_externalEdges.obj"};
  Info << "Writing external edges to " << externalStr.name() << endl;
  for (label i = externalStart_; i < internalStart_; i++) {
    const edge& e = edges()[i];
    externalStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream internalStr{prefix + "_internalEdges.obj"};
  Info << "Writing internal edges to " << internalStr.name() << endl;
  for (label i = internalStart_; i < flatStart_; i++) {
    const edge& e = edges()[i];
    internalStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream flatStr{prefix + "_flatEdges.obj"};
  Info << "Writing flat edges to " << flatStr.name() << endl;
  for (label i = flatStart_; i < openStart_; i++) {
    const edge& e = edges()[i];
    flatStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream openStr{prefix + "_openEdges.obj"};
  Info << "Writing open edges to " << openStr.name() << endl;
  for (label i = openStart_; i < multipleStart_; i++) {
    const edge& e = edges()[i];
    openStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream multipleStr{prefix + "_multipleEdges.obj"};
  Info << "Writing multiple edges to " << multipleStr.name() << endl;
  for (label i = multipleStart_; i < edges().size(); i++) {
    const edge& e = edges()[i];
    multipleStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream regionStr{prefix + "_regionEdges.obj"};
  Info << "Writing region edges to " << regionStr.name() << endl;
  FOR_ALL(regionEdges_, i) {
    const edge& e = edges()[regionEdges_[i]];
    regionStr.write(linePointRef(points()[e[0]], points()[e[1]]));
  }
  OBJstream edgeDirsStr{prefix + "_edgeDirections.obj"};
  Info << "Writing edge directions to " << edgeDirsStr.name() << endl;
  FOR_ALL(edgeDirections_, i) {
    const vector& eVec = edgeDirections_[i];
    const edge& e = edges()[i];
    edgeDirsStr.write
    (
      linePointRef(points()[e.start()], eVec + points()[e.start()])
    );
  }
}


void mousse::extendedEdgeMesh::writeStats(Ostream& os) const
{
  edgeMesh::writeStats(os);
  os << indent << "point classification :" << nl;
  os << incrIndent;
  os << indent << "convex feature points          : "
    << setw(8) << concaveStart_-convexStart_
    //<< setw(8) << convexStart_
    << nl;
  os << indent << "concave feature points         : "
    << setw(8) << mixedStart_-concaveStart_
    //<< setw(8) << concaveStart_
    << nl;
  os << indent << "mixed feature points           : "
    << setw(8) << nonFeatureStart_-mixedStart_
    //<< setw(8) << mixedStart_
    << nl;
  os << indent << "other (non-feature) points     : "
    << setw(8) << points().size()-nonFeatureStart_
    //<< setw(8) << nonFeatureStart_
    << nl;
  os << decrIndent;
  os << indent << "edge classification :" << nl;
  os << incrIndent;
  os << indent << "external (convex angle) edges  : "
    << setw(8) << internalStart_-externalStart_
    //<< setw(8) << externalStart_
    << nl;
  os << indent << "internal (concave angle) edges : "
    << setw(8) << flatStart_-internalStart_
    //<< setw(8) << internalStart_
    << nl;
  os << indent << "flat region edges              : "
    << setw(8) << openStart_-flatStart_
    //<< setw(8) << flatStart_
    << nl;
  os << indent << "open edges                     : "
    << setw(8) << multipleStart_-openStart_
    //<< setw(8) << openStart_
    << nl;
  os << indent << "multiply connected edges       : "
    << setw(8) << edges().size()-multipleStart_
    //<< setw(8) << multipleStart_
    << nl;
  os << decrIndent;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>
(
  Istream& is,
  mousse::extendedEdgeMesh::sideVolumeType& vt
)
{
  label type;
  is >> type;
  vt = static_cast<mousse::extendedEdgeMesh::sideVolumeType>(type);
  // Check state of Istream
  is.check("operator>>(Istream&, sideVolumeType&)");
  return is;
}


mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const mousse::extendedEdgeMesh::sideVolumeType& vt
)
{
  os << static_cast<label>(vt);
  return os;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const extendedEdgeMesh& em)
{
  os << "// points" << nl
    << em.points() << nl
    << "// edges" << nl
    << em.edges() << nl
    << "// concaveStart mixedStart nonFeatureStart" << nl
    << em.concaveStart_ << token::SPACE
    << em.mixedStart_ << token::SPACE
    << em.nonFeatureStart_ << nl
    << "// internalStart flatStart openStart multipleStart" << nl
    << em.internalStart_ << token::SPACE
    << em.flatStart_ << token::SPACE
    << em.openStart_ << token::SPACE
    << em.multipleStart_ << nl
    << "// normals" << nl
    << em.normals_ << nl
    << "// normal volume types" << nl
    << em.normalVolumeTypes_ << nl
    << "// normalDirections" << nl
    << em.normalDirections_ << nl
    << "// edgeNormals" << nl
    << em.edgeNormals_ << nl
    << "// featurePointNormals" << nl
    << em.featurePointNormals_ << nl
    << "// featurePointEdges" << nl
    << em.featurePointEdges_ << nl
    << "// regionEdges" << nl
    << em.regionEdges_
    << endl;
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const extendedEdgeMesh&)");
  return os;
}


mousse::Istream& mousse::operator>>(Istream& is, extendedEdgeMesh& em)
{
  is >> static_cast<edgeMesh&>(em)
    >> em.concaveStart_
    >> em.mixedStart_
    >> em.nonFeatureStart_
    >> em.internalStart_
    >> em.flatStart_
    >> em.openStart_
    >> em.multipleStart_
    >> em.normals_
    >> em.normalVolumeTypes_
    >> em.normalDirections_
    >> em.edgeNormals_
    >> em.featurePointNormals_
    >> em.featurePointEdges_
    >> em.regionEdges_;
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, extendedEdgeMesh&)");
  return is;
}

