// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "delaunay_mesh.hpp"
#include "fv_mesh.hpp"
#include "point_conversion.hpp"
#include "wall_poly_patch.hpp"
#include "processor_poly_patch.hpp"
#include "label_io_field.hpp"


// Private Member Functions 
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::sortFaces
(
  faceList& faces,
  labelList& owner,
  labelList& neighbour
) const
{
  // Upper triangular order:
  // + owner is sorted in ascending cell order
  // + within each block of equal value for owner, neighbour is sorted in
  //   ascending cell order.
  // + faces sorted to correspond
  // e.g.
  // owner | neighbour
  // 0     | 2
  // 0     | 23
  // 0     | 71
  // 1     | 23
  // 1     | 24
  // 1     | 91
  List<labelPair> ownerNeighbourPair{owner.size()};
  FOR_ALL(ownerNeighbourPair, oNI) {
    ownerNeighbourPair[oNI] = labelPair(owner[oNI], neighbour[oNI]);
  }
  Info << nl
    << "Sorting faces, owner and neighbour into upper triangular order"
    << endl;
  labelList oldToNew;
  sortedOrder(ownerNeighbourPair, oldToNew);
  oldToNew = invert(oldToNew.size(), oldToNew);
  inplaceReorder(oldToNew, faces);
  inplaceReorder(oldToNew, owner);
  inplaceReorder(oldToNew, neighbour);
}


template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::addPatches
(
  const label nInternalFaces,
  faceList& faces,
  labelList& owner,
  PtrList<dictionary>& patchDicts,
  const List<DynamicList<face>>& patchFaces,
  const List<DynamicList<label>>& patchOwners
) const
{
  label nPatches = patchFaces.size();
  patchDicts.setSize(nPatches);
  FOR_ALL(patchDicts, patchI) {
    patchDicts.set(patchI, new dictionary{});
  }
  label nBoundaryFaces = 0;
  FOR_ALL(patchFaces, p) {
    patchDicts[p].set("nFaces", patchFaces[p].size());
    patchDicts[p].set("startFace", nInternalFaces + nBoundaryFaces);
    nBoundaryFaces += patchFaces[p].size();
  }
  faces.setSize(nInternalFaces + nBoundaryFaces);
  owner.setSize(nInternalFaces + nBoundaryFaces);
  label faceI = nInternalFaces;
  FOR_ALL(patchFaces, p) {
    FOR_ALL(patchFaces[p], f) {
      faces[faceI] = patchFaces[p][f];
      owner[faceI] = patchOwners[p][f];
      faceI++;
    }
  }
}


// Member Operators 
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::printInfo(Ostream&) const
{
  PrintTable<word, label> triInfoTable{"Mesh Statistics"};
  triInfoTable.add("Points", Triangulation::number_of_vertices());
  triInfoTable.add("Edges", Triangulation::number_of_finite_edges());
  triInfoTable.add("Faces", Triangulation::number_of_finite_facets());
  triInfoTable.add("Cells", Triangulation::number_of_finite_cells());
  scalar minSize = GREAT;
  scalar maxSize = 0;
  for (auto vit = Triangulation::finite_vertices_begin();
       vit != Triangulation::finite_vertices_end();
       ++vit) {
    // Only internal or boundary vertices have a size
    if (vit->internalOrBoundaryPoint()) {
      minSize = min(vit->targetCellSize(), minSize);
      maxSize = max(vit->targetCellSize(), maxSize);
    }
  }
  Info << incrIndent;
  triInfoTable.print(Info, true, true);
  Info << "Size (Min/Max) = "
    << returnReduce(minSize, minOp<scalar>()) << " "
    << returnReduce(maxSize, maxOp<scalar>()) << endl;
  Info << decrIndent;
}


template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::printVertexInfo(Ostream& os) const
{
  label nInternal = 0;
  label nInternalRef = 0;
  label nUnassigned = 0;
  label nUnassignedRef = 0;
  label nInternalNearBoundary = 0;
  label nInternalNearBoundaryRef = 0;
  label nInternalSurface = 0;
  label nInternalSurfaceRef = 0;
  label nInternalFeatureEdge = 0;
  label nInternalFeatureEdgeRef = 0;
  label nInternalFeaturePoint = 0;
  label nInternalFeaturePointRef = 0;
  label nExternalSurface = 0;
  label nExternalSurfaceRef = 0;
  label nExternalFeatureEdge = 0;
  label nExternalFeatureEdgeRef = 0;
  label nExternalFeaturePoint = 0;
  label nExternalFeaturePointRef = 0;
  label nFar = 0;
  label nReferred = 0;
  for (auto vit = Triangulation::finite_vertices_begin();
       vit != Triangulation::finite_vertices_end();
       ++vit) {
    if (vit->type() == Vb::vtInternal) {
      if (vit->referred()) {
        nReferred++;
        nInternalRef++;
      }
      nInternal++;
    } else if (vit->type() == Vb::vtUnassigned) {
      if (vit->referred()) {
        nReferred++;
        nUnassignedRef++;
      }
      nUnassigned++;
    } else if (vit->type() == Vb::vtInternalNearBoundary) {
      if (vit->referred()) {
        nReferred++;
        nInternalNearBoundaryRef++;
      }
      nInternalNearBoundary++;
    } else if (vit->type() == Vb::vtInternalSurface) {
      if (vit->referred()) {
        nReferred++;
        nInternalSurfaceRef++;
      }
      nInternalSurface++;
    } else if (vit->type() == Vb::vtInternalFeatureEdge) {
      if (vit->referred()) {
        nReferred++;
        nInternalFeatureEdgeRef++;
      }
      nInternalFeatureEdge++;
    } else if (vit->type() == Vb::vtInternalFeaturePoint) {
      if (vit->referred()) {
        nReferred++;
        nInternalFeaturePointRef++;
      }
      nInternalFeaturePoint++;
    } else if (vit->type() == Vb::vtExternalSurface) {
      if (vit->referred()) {
        nReferred++;
        nExternalSurfaceRef++;
      }
      nExternalSurface++;
    } else if (vit->type() == Vb::vtExternalFeatureEdge) {
      if (vit->referred()) {
        nReferred++;
        nExternalFeatureEdgeRef++;
      }
      nExternalFeatureEdge++;
    } else if (vit->type() == Vb::vtExternalFeaturePoint) {
      if (vit->referred()) {
        nReferred++;
        nExternalFeaturePointRef++;
      }
      nExternalFeaturePoint++;
    } else if (vit->type() == Vb::vtFar) {
      nFar++;
    }
  }
  label nTotalVertices =
    nUnassigned
    + nInternal
    + nInternalNearBoundary
    + nInternalSurface
    + nInternalFeatureEdge
    + nInternalFeaturePoint
    + nExternalSurface
    + nExternalFeatureEdge
    + nExternalFeaturePoint
    + nFar;
  if (nTotalVertices != label(Triangulation::number_of_vertices())) {
    WARNING_IN("mousse::conformalVoronoiMesh::printVertexInfo()")
      << nTotalVertices << " does not equal "
      << Triangulation::number_of_vertices()
      << endl;
  }
  PrintTable<word, label> vertexTable{"Vertex Type Information"};
  vertexTable.add("Total", nTotalVertices);
  vertexTable.add("Unassigned", nUnassigned);
  vertexTable.add("nInternal", nInternal);
  vertexTable.add("nInternalNearBoundary", nInternalNearBoundary);
  vertexTable.add("nInternalSurface", nInternalSurface);
  vertexTable.add("nInternalFeatureEdge", nInternalFeatureEdge);
  vertexTable.add("nInternalFeaturePoint", nInternalFeaturePoint);
  vertexTable.add("nExternalSurface", nExternalSurface);
  vertexTable.add("nExternalFeatureEdge", nExternalFeatureEdge);
  vertexTable.add("nExternalFeaturePoint", nExternalFeaturePoint);
  vertexTable.add("nFar", nFar);
  vertexTable.add("nReferred", nReferred);
  os << endl;
  vertexTable.print(os);
}


template<class Triangulation>
mousse::autoPtr<mousse::polyMesh>
mousse::DelaunayMesh<Triangulation>::createMesh
(
  const fileName& name,
  labelTolabelPairHashTable& vertexMap,
  labelList& cellMap,
  const bool writeDelaunayData
) const
{
  pointField points{static_cast<label>(Triangulation::number_of_vertices())};
  faceList faces{static_cast<label>(Triangulation::number_of_finite_facets())};
  labelList owner{static_cast<label>(Triangulation::number_of_finite_facets())};
  labelList neighbour
  {
    static_cast<label>(Triangulation::number_of_finite_facets())
  };
  wordList patchNames{1, "foamyHexMesh_defaultPatch"};
  wordList patchTypes{1, wallPolyPatch::typeName};
  PtrList<dictionary> patchDicts{1};
  patchDicts.set(0, new dictionary());
  List<DynamicList<face>> patchFaces{1, DynamicList<face>()};
  List<DynamicList<label>> patchOwners{1, DynamicList<label>()};
  vertexMap.resize(vertexCount());
  cellMap.setSize(Triangulation::number_of_finite_cells(), -1);
  // Calculate pts and a map of point index to location in pts.
  label vertI = 0;
  labelIOField types
  {
    {
      "types",
      time().timeName(),
      name,
      time(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    static_cast<label>(Triangulation::number_of_vertices())
  };
  labelIOField processorIndices
  {
    {
      "processorIndices",
      time().timeName(),
      name,
      time(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    static_cast<label>(Triangulation::number_of_vertices())
  };
  for (auto vit = Triangulation::finite_vertices_begin();
       vit != Triangulation::finite_vertices_end();
       ++vit) {
    if (!vit->farPoint()) {
      vertexMap(labelPair(vit->index(), vit->procIndex())) = vertI;
      points[vertI] = topoint(vit->point());
      types[vertI] = static_cast<label>(vit->type());
      processorIndices[vertI] = vit->procIndex();
      vertI++;
    }
  }
  points.setSize(vertI);
  types.setSize(vertI);
  processorIndices.setSize(vertI);
  // Index the cells
  label cellI = 0;
  for (auto cit = Triangulation::finite_cells_begin();
       cit != Triangulation::finite_cells_end();
       ++cit) {
    if (!cit->hasFarPoint()
        && !Triangulation::is_infinite(cit)
        && cit->real()) {
      cellMap[cit->cellIndex()] = cellI++;
    }
  }
  label faceI = 0;
  labelList verticesOnTriFace(3, label(-1));
  face newFace{verticesOnTriFace};
  for (auto fit = Triangulation::finite_facets_begin();
       fit != Triangulation::finite_facets_end();
       ++fit) {
    const Cell_handle c1{fit->first};
    const label oppositeVertex = fit->second;
    const Cell_handle c2{c1->neighbor(oppositeVertex)};

    label c1I = Cb::ctFar;
    bool c1Real = false;
    if (!Triangulation::is_infinite(c1)
        && !c1->hasFarPoint()
        && c1->real()) {
      c1I = cellMap[c1->cellIndex()];
      c1Real = true;
    }
    label c2I = Cb::ctFar;
    bool c2Real = false;
    if (!Triangulation::is_infinite(c2)
        && !c2->hasFarPoint()
        && c2->real()) {
      c2I = cellMap[c2->cellIndex()];
      c2Real = true;
    }
    if (!c1Real && !c2Real) {
      // Both tets are outside, skip
      continue;
    }
    label ownerCell = -1;
    label neighbourCell = -1;
    for (label i = 0; i < 3; i++) {
      verticesOnTriFace[i] = vertexMap
      [
        labelPair
        (
          c1->vertex
          (
            Triangulation::vertex_triple_index(oppositeVertex, i)
          )->index(),
          c1->vertex
          (
            Triangulation::vertex_triple_index(oppositeVertex, i)
          )->procIndex()
        )
      ];
    }
    newFace = face{verticesOnTriFace};
    if (!c1Real || !c2Real) {
      // Boundary face...
      if (!c1Real) {
        //... with c1 outside
        ownerCell = c2I;
      } else {
        // ... with c2 outside
        ownerCell = c1I;
        reverse(newFace);
      }
      patchFaces[0].append(newFace);
      patchOwners[0].append(ownerCell);
    } else {
      // Internal face...
      if (c1I < c2I) {
        // ...with c1 as the ownerCell
        ownerCell = c1I;
        neighbourCell = c2I;
        reverse(newFace);
      } else {
        // ...with c2 as the ownerCell
        ownerCell = c2I;
        neighbourCell = c1I;
      }
      faces[faceI] = newFace;
      owner[faceI] = ownerCell;
      neighbour[faceI] = neighbourCell;
      faceI++;
    }
  }
  faces.setSize(faceI);
  owner.setSize(faceI);
  neighbour.setSize(faceI);
  sortFaces(faces, owner, neighbour);
  Info << "Creating patches" << endl;
  addPatches
  (
    faceI,
    faces,
    owner,
    patchDicts,
    patchFaces,
    patchOwners
  );
  Info << "Creating mesh" << endl;
  autoPtr<polyMesh> meshPtr
  {
    new polyMesh
    {
      {
        name,
        time().timeName(),
        time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      xferMove(points),
      xferMove(faces),
      xferMove(owner),
      xferMove(neighbour)
    }
  };
  Info << "Adding patches" << endl;
  List<polyPatch*> patches{patchNames.size()};
  label nValidPatches = 0;
  FOR_ALL(patches, p) {
    patches[nValidPatches] =
      polyPatch::New
      (
        patchTypes[p],
        patchNames[p],
        patchDicts[p],
        nValidPatches,
        meshPtr().boundaryMesh()
      ).ptr();
    nValidPatches++;
  }
  patches.setSize(nValidPatches);
  meshPtr().addPatches(patches);
  if (writeDelaunayData) {
    types.write();
    processorIndices.write();
  }
  Info << "Mesh created" << endl;
  return meshPtr;
}

