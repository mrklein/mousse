// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_refinement.hpp"
#include "fv_mesh.hpp"
#include "global_index.hpp"
#include "sync_tools.hpp"


// Member Functions 
// Add a T entry
template<class T> void mousse::meshRefinement::updateList
(
  const labelList& newToOld,
  const T& nullValue,
  List<T>& elems
)
{
  List<T> newElems{newToOld.size(), nullValue};
  FOR_ALL(newElems, i) {
    label oldI = newToOld[i];
    if (oldI >= 0) {
      newElems[i] = elems[oldI];
    }
  }
  elems.transfer(newElems);
}


template<class T>
T mousse::meshRefinement::gAverage
(
  const PackedBoolList& isMasterElem,
  const UList<T>& values
)
{
  if (values.size() != isMasterElem.size()) {
    FATAL_ERROR_IN
    (
      "meshRefinement::gAverage\n"
      "(\n"
      "  const PackedBoolList& isMasterElem,\n"
      "  const UList<T>& values\n"
      ")\n"
    )
    << "Number of elements in list " << values.size()
    << " does not correspond to number of elements in isMasterElem "
    << isMasterElem.size()
    << exit(FatalError);
  }
  T sum = pTraits<T>::zero;
  label n = 0;
  FOR_ALL(values, i) {
    if (isMasterElem[i]) {
      sum += values[i];
      n++;
    }
  }
  reduce(sum, sumOp<T>());
  reduce(n, sumOp<label>());
  if (n > 0) {
    return sum/n;
  } else {
    return pTraits<T>::max;
  }
}


// Compare two lists over all boundary faces
template<class T>
void mousse::meshRefinement::testSyncBoundaryFaceList
(
  const scalar tol,
  const string& msg,
  const UList<T>& faceData,
  const UList<T>& syncedFaceData
) const
{
  label nBFaces = mesh_.nFaces() - mesh_.nInternalFaces();
  if (faceData.size() != nBFaces || syncedFaceData.size() != nBFaces) {
    FATAL_ERROR_IN
    (
      "meshRefinement::testSyncBoundaryFaceList"
      "(const scalar, const string&, const List<T>&, const List<T>&)"
    )
    << "Boundary faces:" << nBFaces
    << " faceData:" << faceData.size()
    << " syncedFaceData:" << syncedFaceData.size()
    << abort(FatalError);
  }
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  FOR_ALL(patches, patchI) {
    const polyPatch& pp = patches[patchI];
    label bFaceI = pp.start() - mesh_.nInternalFaces();
    FOR_ALL(pp, i) {
      const T& data = faceData[bFaceI];
      const T& syncData = syncedFaceData[bFaceI];
      if (mag(data - syncData) > tol) {
        label faceI = pp.start()+i;
        FATAL_ERROR_IN("testSyncFaces")
          << msg
          << "patchFace:" << i
          << " face:" << faceI
          << " fc:" << mesh_.faceCentres()[faceI]
          << " patch:" << pp.name()
          << " faceData:" << data
          << " syncedFaceData:" << syncData
          << " diff:" << mag(data - syncData)
          << abort(FatalError);
      }
      bFaceI++;
    }
  }
}


// Print list sorted by coordinates. Used for comparing non-parallel v.s.
// parallel operation
template<class T>
void mousse::meshRefinement::collectAndPrint
(
  const UList<point>& points,
  const UList<T>& data
)
{
  globalIndex globalPoints{points.size()};
  pointField allPoints;
  globalPoints.gather
    (
      Pstream::worldComm,
      identity(Pstream::nProcs()),
      points,
      allPoints,
      UPstream::msgType(),
      Pstream::blocking
    );
  List<T> allData;
  globalPoints.gather
    (
      Pstream::worldComm,
      identity(Pstream::nProcs()),
      data,
      allData,
      UPstream::msgType(),
      Pstream::blocking
    );
  scalarField magAllPoints{mag(allPoints-point(-0.317, 0.117, 0.501))};
  labelList visitOrder;
  sortedOrder(magAllPoints, visitOrder);
  FOR_ALL(visitOrder, i) {
    label allPointI = visitOrder[i];
    Info << allPoints[allPointI] << " : " << allData[allPointI] << endl;
  }
}


//template<class T, class Mesh>
template<class GeoField>
void mousse::meshRefinement::addPatchFields
(
  fvMesh& mesh,
  const word& patchFieldType
)
{
  HashTable<GeoField*> flds{mesh.objectRegistry::lookupClass<GeoField>()};
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter) {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld = fld.boundaryField();
    label sz = bfld.size();
    bfld.setSize(sz+1);
    bfld.set
    (
      sz,
      GeoField::PatchFieldType::New
      (
        patchFieldType,
        mesh.boundary()[sz],
        fld.dimensionedInternalField()
      )
    );
  }
}


// Reorder patch field
template<class GeoField>
void mousse::meshRefinement::reorderPatchFields
(
  fvMesh& mesh,
  const labelList& oldToNew
)
{
  HashTable<GeoField*> flds{mesh.objectRegistry::lookupClass<GeoField>()};
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter) {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld = fld.boundaryField();
    bfld.reorder(oldToNew);
  }
}


template<class Enum>
int mousse::meshRefinement::readFlags
(
  const Enum& namedEnum,
  const wordList& words
)
{
  int flags = 0;
  FOR_ALL(words, i) {
    int index = namedEnum[words[i]];
    int val = 1<<index;
    flags |= val;
  }
  return flags;
}


template<class Type>
void mousse::meshRefinement::weightedSum
(
  const polyMesh& mesh,
  const PackedBoolList& isMasterEdge,
  const labelList& meshPoints,
  const edgeList& edges,
  const scalarField& edgeWeights,
  const Field<Type>& pointData,
  Field<Type>& sum
)
{
  if (edges.size() != isMasterEdge.size()
      || edges.size() != edgeWeights.size()
      || meshPoints.size() != pointData.size()) {
    FATAL_ERROR_IN("medialAxisMeshMover::weightedSum(..)")
      << "Inconsistent sizes for edge or point data:"
      << " isMasterEdge:" << isMasterEdge.size()
      << " edgeWeights:" << edgeWeights.size()
      << " edges:" << edges.size()
      << " pointData:" << pointData.size()
      << " meshPoints:" << meshPoints.size()
      << abort(FatalError);
  }
  sum.setSize(meshPoints.size());
  sum = pTraits<Type>::zero;
  FOR_ALL(edges, edgeI) {
    if (isMasterEdge[edgeI]) {
      const edge& e = edges[edgeI];
      scalar eWeight = edgeWeights[edgeI];
      label v0 = e[0];
      label v1 = e[1];
      sum[v0] += eWeight*pointData[v1];
      sum[v1] += eWeight*pointData[v0];
    }
  }
  syncTools::syncPointList
    (
      mesh,
      meshPoints,
      sum,
      plusEqOp<Type>(),
      pTraits<Type>::zero     // null value
    );
}

