// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conformal_voronoi_mesh.hpp"
#include "motion_smoother.hpp"
#include "background_mesh_decomposition.hpp"
#include "poly_mesh_geometry.hpp"
#include "indexed_cell_checks.hpp"
#include "obj_stream.hpp"
#include "indexed_cell_ops.hpp"
#include "list_ops.hpp"
#include "delaunay_mesh_tools.hpp"


// Private Member Functions 
void mousse::conformalVoronoiMesh::calcDualMesh
(
  pointField& points,
  labelList& boundaryPts,
  faceList& faces,
  labelList& owner,
  labelList& neighbour,
  wordList& patchNames,
  PtrList<dictionary>& patchDicts,
  pointField& cellCentres,
  labelList& cellToDelaunayVertex,
  labelListList& patchToDelaunayVertex,
  PackedBoolList& boundaryFacesToRemove
)
{
  timeCheck("Start calcDualMesh");
  setVertexSizeAndAlignment();
  timeCheck("After setVertexSizeAndAlignment");
  indexDualVertices(points, boundaryPts);

  {
    Info << nl << "Merging identical points" << endl;
    // There is no guarantee that a merge of close points is no-risk
    mergeIdenticalDualVertices(points, boundaryPts);
  }

  // Final dual face and owner neighbour construction
  timeCheck("Before createFacesOwnerNeighbourAndPatches");
  createFacesOwnerNeighbourAndPatches
    (
      points,
      faces,
      owner,
      neighbour,
      patchNames,
      patchDicts,
      patchToDelaunayVertex,  // from patch face to Delaunay vertex (slavePp)
      boundaryFacesToRemove,
      false
    );
  // deferredCollapseFaceSet(owner, neighbour, deferredCollapseFaces);
  cellCentres = DelaunayMeshTools::allPoints(*this);
  cellToDelaunayVertex = removeUnusedCells(owner, neighbour);
  cellCentres = pointField(cellCentres, cellToDelaunayVertex);
  removeUnusedPoints(faces, points, boundaryPts);
  timeCheck("End of calcDualMesh");
}


void mousse::conformalVoronoiMesh::calcTetMesh
(
  pointField& points,
  labelList& pointToDelaunayVertex,
  faceList& faces,
  labelList& owner,
  labelList& neighbour,
  wordList& patchNames,
  PtrList<dictionary>& /*patchDicts*/
)
{
  const label isz = static_cast<label>(number_of_vertices());
  labelList vertexMap{isz};
  label vertI = 0;
  points.setSize(isz);
  pointToDelaunayVertex.setSize(isz);
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit) {
    if (vit->internalPoint() || vit->boundaryPoint()) {
      vertexMap[vit->index()] = vertI;
      points[vertI] = topoint(vit->point());
      pointToDelaunayVertex[vertI] = vit->index();
      vertI++;
    }
  }
  points.setSize(vertI);
  pointToDelaunayVertex.setSize(vertI);
  label cellI = 0;
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit) {
    if (cit->internalOrBoundaryDualVertex()) {
      cit->cellIndex() = cellI++;
    } else {
      cit->cellIndex() = Cb::ctFar;
    }
  }
  patchNames = geometryToConformTo_.patchNames();
  patchNames.setSize(patchNames.size() + 1);
  patchNames[patchNames.size() - 1] = "foamyHexMesh_defaultPatch";
  label nPatches = patchNames.size();
  List<DynamicList<face>> patchFaces{nPatches, DynamicList<face>{0}};
  List<DynamicList<label>> patchOwners{nPatches, DynamicList<label>{0}};
  faces.setSize(number_of_finite_facets());
  owner.setSize(number_of_finite_facets());
  neighbour.setSize(number_of_finite_facets());
  label faceI = 0;
  labelList verticesOnTriFace{3, label{-1}};
  face newFace{verticesOnTriFace};
  for (auto fit = finite_facets_begin();
       fit != finite_facets_end();
       ++fit) {
    const Cell_handle c1{fit->first};
    const label oppositeVertex = fit->second;
    const Cell_handle c2{c1->neighbor(oppositeVertex)};
    if (c1->hasFarPoint() && c2->hasFarPoint()) {
      // Both tets are outside, skip
      continue;
    }
    label c1I = c1->cellIndex();
    label c2I = c2->cellIndex();
    label ownerCell = -1;
    label neighbourCell = -1;
    for (label i = 0; i < 3; i++) {
      verticesOnTriFace[i] =
        vertexMap[c1->vertex(vertex_triple_index(oppositeVertex, i))->index()];
    }
    newFace = face{verticesOnTriFace};
    if (c1->hasFarPoint() || c2->hasFarPoint()) {
      // Boundary face...
      if (c1->hasFarPoint()) {
        //... with c1 outside
        ownerCell = c2I;
      } else {
        // ... with c2 outside
        ownerCell = c1I;
        reverse(newFace);
      }
      label patchIndex =
        geometryToConformTo_.findPatch(newFace.centre(points));
      if (patchIndex == -1) {
        patchIndex = patchNames.size() - 1;
        WARNING_IN("mousse::conformalVoronoiMesh::calcTetMesh")
          << "Tet face centre at  " << nl
          << newFace.centre(points) << nl
          << "did not find a surface patch. Adding to "
          << patchNames[patchIndex]
          << endl;
      }
      patchFaces[patchIndex].append(newFace);
      patchOwners[patchIndex].append(ownerCell);
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
  label nInternalFaces = faceI;
  faces.setSize(nInternalFaces);
  owner.setSize(nInternalFaces);
  neighbour.setSize(nInternalFaces);
  sortFaces(faces, owner, neighbour);
}


void mousse::conformalVoronoiMesh::mergeIdenticalDualVertices
(
  const pointField& pts,
  labelList& boundaryPts
)
{
  // Assess close points to be merged
  label nPtsMerged = 0;
  label nPtsMergedSum = 0;
  do {
    Map<label> dualPtIndexMap;
    nPtsMerged =
      mergeIdenticalDualVertices(pts, dualPtIndexMap);
    reindexDualVertices(dualPtIndexMap, boundaryPts);
    reduce(nPtsMerged, sumOp<label>());
    nPtsMergedSum += nPtsMerged;
  } while (nPtsMerged > 0);
  if (nPtsMergedSum > 0) {
    Info << "    Merged " << nPtsMergedSum << " points " << endl;
  }
}


mousse::label mousse::conformalVoronoiMesh::mergeIdenticalDualVertices
(
  const pointField& pts,
  Map<label>& dualPtIndexMap
) const
{
  label nPtsMerged = 0;
  for (auto fit = finite_facets_begin();
       fit != finite_facets_end();
       ++fit) {
    const Cell_handle c1{fit->first};
    const label oppositeVertex = fit->second;
    const Cell_handle c2{c1->neighbor(oppositeVertex)};
    if (is_infinite(c1) || is_infinite(c2)) {
      continue;
    }
    label& c1I = c1->cellIndex();
    label& c2I = c2->cellIndex();
    if ((c1I == c2I) || c1->hasFarPoint() || c2->hasFarPoint())
      continue;
    const mousse::point& p1 = pts[c1I];
    const mousse::point& p2 = pts[c2I];
    if (p1 != p2)
      continue;
    label& second_index = (c1I < c2I) ? c1I : c2I;
    dualPtIndexMap.insert(c1I, second_index);
    dualPtIndexMap.insert(c2I, second_index);
    nPtsMerged++;
  }
  if (debug) {
    Info << "mergeIdenticalDualVertices:" << endl
      << "    zero-length edges     : "
      << returnReduce(nPtsMerged, sumOp<label>()) << endl
      << endl;
  }
  return nPtsMerged;
}


void mousse::conformalVoronoiMesh::deferredCollapseFaceSet
(
  labelList& owner,
  labelList& neighbour,
  const HashSet<labelPair, labelPair::Hash<>>& deferredCollapseFaces
) const
{
  DynamicList<label> faceLabels;
  FOR_ALL(neighbour, nI) {
    if (deferredCollapseFaces.found(Pair<label>(owner[nI], neighbour[nI]))) {
      faceLabels.append(nI);
    }
  }
  Pout << "facesToCollapse" << nl << faceLabels << endl;
}


mousse::autoPtr<mousse::polyMesh>
mousse::conformalVoronoiMesh::createPolyMeshFromPoints
(
  const pointField& pts
) const
{
  faceList faces;
  labelList owner;
  labelList neighbour;
  wordList patchNames;
  PtrList<dictionary> patchDicts;
  pointField cellCentres;
  labelListList patchToDelaunayVertex;
  PackedBoolList boundaryFacesToRemove;
  timeCheck("Start of checkPolyMeshQuality");
  Info << nl << "Creating polyMesh to assess quality" << endl;
  createFacesOwnerNeighbourAndPatches
    (
      pts,
      faces,
      owner,
      neighbour,
      patchNames,
      patchDicts,
      patchToDelaunayVertex,
      boundaryFacesToRemove,
      false
    );
  cellCentres = DelaunayMeshTools::allPoints(*this);
  labelList cellToDelaunayVertex{removeUnusedCells(owner, neighbour)};
  cellCentres = pointField{cellCentres, cellToDelaunayVertex};
  autoPtr<polyMesh> meshPtr
  {
    new polyMesh
    {
      IOobject
      {
        "foamyHexMesh_temporary",
        runTime_.timeName(),
        runTime_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      xferCopy(pts),
      xferMove(faces),
      xferMove(owner),
      xferMove(neighbour)
    }
  };
  polyMesh& pMesh = meshPtr();
  List<polyPatch*> patches{patchNames.size()};
  label nValidPatches = 0;
  FOR_ALL(patches, p) {
    label totalPatchSize = readLabel(patchDicts[p].lookup("nFaces"));
    if (patchDicts.set(p)
        && word{patchDicts[p].lookup("type")} == processorPolyPatch::typeName) {
      // Do not create empty processor patches
      if (totalPatchSize > 0) {
        patchDicts[p].set("transform", "coincidentFullMatch");
        patches[nValidPatches] =
          new processorPolyPatch
          {
            patchNames[p],
            patchDicts[p],
            nValidPatches,
            pMesh.boundaryMesh(),
            processorPolyPatch::typeName
          };
        nValidPatches++;
      }
    } else {
      // Check that the patch is not empty on every processor
      reduce(totalPatchSize, sumOp<label>());
      if (totalPatchSize > 0) {
        patches[nValidPatches] =
          polyPatch::New
          (
            patchNames[p],
            patchDicts[p],
            nValidPatches,
            pMesh.boundaryMesh()
          ).ptr();
        nValidPatches++;
      }
    }
  }
  patches.setSize(nValidPatches);
  pMesh.addPatches(patches);
  return meshPtr;
}


void mousse::conformalVoronoiMesh::checkCellSizing()
{
  Info << "Checking cell sizes..."<< endl;
  timeCheck("Start of Cell Sizing");
  labelList boundaryPts{static_cast<label>(number_of_finite_cells()), internal};
  pointField ptsField;
  indexDualVertices(ptsField, boundaryPts);
  // Merge close dual vertices.
  mergeIdenticalDualVertices(ptsField, boundaryPts);
  autoPtr<polyMesh> meshPtr = createPolyMeshFromPoints(ptsField);
  const polyMesh& pMesh = meshPtr();
  //pMesh.write();
  // Find cells with poor quality
  DynamicList<label> checkFaces{identity(pMesh.nFaces())};
  labelHashSet wrongFaces{pMesh.nFaces()/100};
  Info << "Running checkMesh on mesh with " << pMesh.nCells()
    << " cells "<< endl;
  const dictionary& dict = foamyHexMeshControls().foamyHexMeshDict();
  const dictionary& meshQualityDict = dict.subDict("meshQualityControls");
  const scalar maxNonOrtho =
    readScalar(meshQualityDict.lookup("maxNonOrtho", true));
  label nWrongFaces = 0;
  if (maxNonOrtho < 180.0 - SMALL) {
    polyMeshGeometry::checkFaceDotProduct
    (
      false,
      maxNonOrtho,
      pMesh,
      pMesh.cellCentres(),
      pMesh.faceAreas(),
      checkFaces,
      List<labelPair>{},
      &wrongFaces
    );
    label nNonOrthogonal = returnReduce(wrongFaces.size(), sumOp<label>());
    Info << "    non-orthogonality > " << maxNonOrtho
      << " degrees : " << nNonOrthogonal << endl;
    nWrongFaces += nNonOrthogonal;
  }
  labelHashSet protrudingCells = findOffsetPatchFaces(pMesh, 0.25);
  label nProtrudingCells = protrudingCells.size();
  Info << "    protruding/intruding cells : " << nProtrudingCells << endl;
  nWrongFaces += nProtrudingCells;
  Info << "    Found total of " << nWrongFaces << " bad faces" << endl;

  {
    labelHashSet cellsToResizeMap{pMesh.nFaces()/100};
    // Find cells that are attached to the faces in wrongFaces.
    FOR_ALL_CONST_ITER(labelHashSet, wrongFaces, iter) {
      const label faceOwner = pMesh.faceOwner()[iter.key()];
      const label faceNeighbour = pMesh.faceNeighbour()[iter.key()];
      if (!cellsToResizeMap.found(faceOwner)) {
        cellsToResizeMap.insert(faceOwner);
      }
      if (!cellsToResizeMap.found(faceNeighbour)) {
        cellsToResizeMap.insert(faceNeighbour);
      }
    }
    cellsToResizeMap += protrudingCells;
    pointField cellsToResize{cellsToResizeMap.size()};
    label count = 0;
    for (label cellI = 0; cellI < pMesh.nCells(); ++cellI) {
      if (cellsToResizeMap.found(cellI)) {
        cellsToResize[count++] = pMesh.cellCentres()[cellI];
      }
    }
    Info << "    DISABLED: Automatically re-sizing " << cellsToResize.size()
      << " cells that are attached to the bad faces: " << endl;
  }
  timeCheck("End of Cell Sizing");
  Info << "Finished checking cell sizes"<< endl;
}


mousse::labelHashSet mousse::conformalVoronoiMesh::findOffsetPatchFaces
(
  const polyMesh& mesh,
  const scalar allowedOffset
) const
{
  timeCheck("Start findRemainingProtrusionSet");
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  cellSet offsetBoundaryCells
  {
    mesh,
    "foamyHexMesh_protrudingCells",
    mesh.nCells()/1000
  };
  FOR_ALL(patches, patchI) {
    const polyPatch& patch = patches[patchI];
    const faceList& localFaces = patch.localFaces();
    const pointField& localPoints = patch.localPoints();
    const labelList& fCell = patch.faceCells();
    FOR_ALL(localFaces, pLFI) {
      const face& f = localFaces[pLFI];
      const mousse::point& faceCentre = f.centre(localPoints);
      const scalar targetSize = targetCellSize(faceCentre);
      pointIndexHit pHit;
      label surfHit = -1;
      geometryToConformTo_.findSurfaceNearest
        (
          faceCentre,
          sqr(targetSize),
          pHit,
          surfHit
        );
      if (pHit.hit()
          && (mag(pHit.hitPoint() - faceCentre) > allowedOffset*targetSize)) {
        offsetBoundaryCells.insert(fCell[pLFI]);
      }
    }
  }
  if (foamyHexMeshControls().objOutput()) {
    offsetBoundaryCells.write();
  }
  return offsetBoundaryCells;
}


mousse::labelHashSet mousse::conformalVoronoiMesh::checkPolyMeshQuality
(
  const pointField& pts
) const
{
  autoPtr<polyMesh> meshPtr = createPolyMeshFromPoints(pts);
  polyMesh& pMesh = meshPtr();
  timeCheck("polyMesh created, checking quality");
  labelHashSet wrongFaces{pMesh.nFaces()/100};
  DynamicList<label> checkFaces{pMesh.nFaces()};
  const vectorField& fAreas = pMesh.faceAreas();
  scalar faceAreaLimit = SMALL;
  FOR_ALL(fAreas, fI) {
    if (mag(fAreas[fI]) > faceAreaLimit) {
      checkFaces.append(fI);
    }
  }
  Info << nl << "Excluding "
    << returnReduce(fAreas.size() - checkFaces.size(), sumOp<label>())
    << " faces from check, < " << faceAreaLimit << " area" << endl;
  const dictionary& dict = foamyHexMeshControls().foamyHexMeshDict();
  const dictionary& meshQualityDict = dict.subDict("meshQualityControls");
  motionSmoother::checkMesh
    (
      false,
      pMesh,
      meshQualityDict,
      checkFaces,
      wrongFaces
    );
  {
    // Check for cells with more than 1 but fewer than 4 faces
    label nInvalidPolyhedra = 0;
    const cellList& cells = pMesh.cells();
    FOR_ALL(cells, cI) {
      if (cells[cI].size() < 4 && cells[cI].size() > 0) {
        nInvalidPolyhedra++;
        FOR_ALL(cells[cI], cFI) {
          wrongFaces.insert(cells[cI][cFI]);
        }
      }
    }
    Info << "    cells with more than 1 but fewer than 4 faces          : "
      << returnReduce(nInvalidPolyhedra, sumOp<label>())
      << endl;
    // Check for cells with one internal face only
    labelList nInternalFaces{pMesh.nCells(), label(0)};
    for (label fI = 0; fI < pMesh.nInternalFaces(); fI++) {
      nInternalFaces[pMesh.faceOwner()[fI]]++;
      nInternalFaces[pMesh.faceNeighbour()[fI]]++;
    }
    const polyBoundaryMesh& patches = pMesh.boundaryMesh();
    FOR_ALL(patches, patchI) {
      if (patches[patchI].coupled()) {
        const labelUList& owners = patches[patchI].faceCells();
        FOR_ALL(owners, i) {
          nInternalFaces[owners[i]]++;
        }
      }
    }
    label oneInternalFaceCells = 0;
    FOR_ALL(nInternalFaces, cI) {
      if (nInternalFaces[cI] <= 1) {
        oneInternalFaceCells++;
        FOR_ALL(cells[cI], cFI) {
          wrongFaces.insert(cells[cI][cFI]);
        }
      }
    }
    Info << "    cells with with zero or one non-boundary face          : "
      << returnReduce(oneInternalFaceCells, sumOp<label>())
      << endl;
  }
  PackedBoolList ptToBeLimited{pts.size(), false};
  FOR_ALL_CONST_ITER(labelHashSet, wrongFaces, iter) {
    const face f = pMesh.faces()[iter.key()];
    FOR_ALL(f, fPtI) {
      ptToBeLimited[f[fPtI]] = true;
    }
  }
  // Apply Delaunay cell filterCounts and determine the maximum
  // overall filterCount
  label maxFilterCount = 0;
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit) {
    label cI = cit->cellIndex();
    if (cI < 0)
      continue;
    if (ptToBeLimited[cI] == true) {
      cit->filterCount()++;
    }
    if (cit->filterCount() > maxFilterCount) {
      maxFilterCount = cit->filterCount();
    }
  }
  Info << nl << "Maximum number of filter limits applied: "
    << returnReduce(maxFilterCount, maxOp<label>()) << endl;
  return wrongFaces;
}


mousse::label mousse::conformalVoronoiMesh::classifyBoundaryPoint
(
  Cell_handle cit
) const
{
  if (cit->boundaryDualVertex()) {
    if (cit->featurePointDualVertex()) {
      return featurePoint;
    } else if (cit->featureEdgeDualVertex()) {
      return featureEdge;
    } else {
      return surface;
    }
  } else if (cit->baffleSurfaceDualVertex()) {
    return surface;
  } else if (cit->baffleEdgeDualVertex()) {
    return featureEdge;
  } else {
    return internal;
  }
}


void mousse::conformalVoronoiMesh::indexDualVertices
(
  pointField& pts,
  labelList& boundaryPts
)
{
  // Indexing Delaunay cells, which are the dual vertices
  this->resetCellCount();
  label nConstrainedVertices = 0;
  if (foamyHexMeshControls().guardFeaturePoints()) {
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         ++vit) {
      if (vit->constrained()) {
        vit->index() = number_of_finite_cells() + nConstrainedVertices;
        nConstrainedVertices++;
      }
    }
  }
  pts.setSize(number_of_finite_cells() + nConstrainedVertices);
  boundaryPts.setSize
    (
      number_of_finite_cells() + nConstrainedVertices,
      internal
    );
  if (foamyHexMeshControls().guardFeaturePoints()) {
    nConstrainedVertices = 0;
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         ++vit) {
      if (!vit->constrained())
        continue;
      pts[number_of_finite_cells() + nConstrainedVertices] =
        topoint(vit->point());
      boundaryPts[number_of_finite_cells() + nConstrainedVertices] =
        constrained;
      nConstrainedVertices++;
    }
  }
  OBJstream snapping1{"snapToSurface1.obj"};
  OBJstream snapping2{"snapToSurface2.obj"};
  OFstream tetToSnapTo{"tetsToSnapTo.obj"};
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit) {
    if (!cit->hasFarPoint()) {
      cit->cellIndex() = getNewCellIndex();
      // For nearly coplanar Delaunay cells that are present on different
      // processors the result of the circumcentre calculation depends on
      // the ordering of the vertices, so synchronise it across processors
      if (Pstream::parRun() && cit->parallelDualVertex()) {
        typedef CGAL::Exact_predicates_exact_constructions_kernel Exact;
        typedef CGAL::Point_3<Exact> ExactPoint;
        List<labelPair> cellVerticesPair{4};
        List<ExactPoint> cellVertices{4};
        for (label vI = 0; vI < 4; ++vI) {
          cellVerticesPair[vI] =
            labelPair{cit->vertex(vI)->procIndex(), cit->vertex(vI)->index()};
          cellVertices[vI] =
            ExactPoint
            (
              cit->vertex(vI)->point().x(),
              cit->vertex(vI)->point().y(),
              cit->vertex(vI)->point().z()
            );
        }
        // Sort the vertices so that they will be in the same order on
        // each processor
        labelList oldToNew;
        sortedOrder(cellVerticesPair, oldToNew);
        oldToNew = invert(oldToNew.size(), oldToNew);
        inplaceReorder(oldToNew, cellVertices);
        ExactPoint synchronisedDual =
          CGAL::circumcenter
          (
            cellVertices[0],
            cellVertices[1],
            cellVertices[2],
            cellVertices[3]
          );
        pts[cit->cellIndex()] =
          mousse::point
          {
            CGAL::to_double(synchronisedDual.x()),
            CGAL::to_double(synchronisedDual.y()),
            CGAL::to_double(synchronisedDual.z())
          };
      } else {
        pts[cit->cellIndex()] = cit->dual();
      }
      // Feature point snapping
      if (foamyHexMeshControls().snapFeaturePoints()) {
        if (cit->featurePointDualVertex()) {
          pointFromPoint dual = cit->dual();
          pointIndexHit fpHit;
          label featureHit;
          // Find nearest feature point and compare
          geometryToConformTo_.findFeaturePointNearest
            (
              dual,
              sqr(targetCellSize(dual)),
              fpHit,
              featureHit
            );
          if (fpHit.hit()) {
            if (debug) {
              Info << "Dual        = " << dual << nl
                << "    Nearest = " << fpHit.hitPoint() << endl;
            }
            pts[cit->cellIndex()] = fpHit.hitPoint();
          }
        }
      }
      boundaryPts[cit->cellIndex()] = classifyBoundaryPoint(cit);
    } else {
      cit->cellIndex() = Cb::ctFar;
    }
  }
}


void mousse::conformalVoronoiMesh::reindexDualVertices
(
  const Map<label>& dualPtIndexMap,
  labelList& boundaryPts
)
{
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit) {
    if (dualPtIndexMap.found(cit->cellIndex())) {
      cit->cellIndex() = dualPtIndexMap[cit->cellIndex()];
      boundaryPts[cit->cellIndex()] =
        max
        (
          boundaryPts[cit->cellIndex()],
          boundaryPts[dualPtIndexMap[cit->cellIndex()]]
        );
    }
  }
}


mousse::label mousse::conformalVoronoiMesh::createPatchInfo
(
  wordList& patchNames,
  PtrList<dictionary>& patchDicts
) const
{
  patchNames = geometryToConformTo_.patchNames();
  patchDicts.setSize(patchNames.size() + 1);
  const PtrList<dictionary>& patchInfo = geometryToConformTo_.patchInfo();
  FOR_ALL(patchNames, patchI) {
    if (patchInfo.set(patchI)) {
      patchDicts.set(patchI, new dictionary{patchInfo[patchI]});
    } else {
      patchDicts.set(patchI, new dictionary{});
      patchDicts[patchI].set
      (
        "type",
        wallPolyPatch::typeName
      );
    }
  }
  patchNames.setSize(patchNames.size() + 1);
  label defaultPatchIndex = patchNames.size() - 1;
  patchNames[defaultPatchIndex] = "foamyHexMesh_defaultPatch";
  patchDicts.set(defaultPatchIndex, new dictionary{});
  patchDicts[defaultPatchIndex].set
  (
    "type",
    wallPolyPatch::typeName
  );
  label nProcPatches = 0;
  if (Pstream::parRun()) {
    List<boolList> procUsedList
    {
      Pstream::nProcs(),
      boolList{Pstream::nProcs(), false}
    };
    boolList& procUsed = procUsedList[Pstream::myProcNo()];
    // Determine which processor patches are required
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         vit++) {
      // This test is not sufficient if one of the processors does
      // not receive a referred vertex from another processor, but does
      // send one to the other processor.
      if (vit->referred()) {
        procUsed[vit->procIndex()] = true;
      }
    }
    // Because the previous test was insufficient, combine the lists.
    Pstream::gatherList(procUsedList);
    Pstream::scatterList(procUsedList);
    FOR_ALL(procUsedList, procI) {
      if (procI != Pstream::myProcNo()) {
        if (procUsedList[procI][Pstream::myProcNo()]) {
          procUsed[procI] = true;
        }
      }
    }
    FOR_ALL(procUsed, pUI) {
      if (procUsed[pUI]) {
        nProcPatches++;
      }
    }
    label nNonProcPatches = patchNames.size();
    label nTotalPatches = nNonProcPatches + nProcPatches;
    patchNames.setSize(nTotalPatches);
    patchDicts.setSize(nTotalPatches);
    for (label pI = nNonProcPatches; pI < nTotalPatches; ++pI) {
      patchDicts.set(pI, new dictionary{});
    }
    label procAddI = 0;
    FOR_ALL(procUsed, pUI) {
      if (procUsed[pUI]) {
        patchNames[nNonProcPatches + procAddI] =
          "procBoundary" + name(Pstream::myProcNo()) + "to" + name(pUI);
        patchDicts[nNonProcPatches + procAddI].set
        (
          "type",
          processorPolyPatch::typeName
        );
        patchDicts[nNonProcPatches + procAddI].set
        (
          "myProcNo",
          Pstream::myProcNo()
        );
        patchDicts[nNonProcPatches + procAddI].set("neighbProcNo", pUI);
        procAddI++;
      }
    }
  }
  return defaultPatchIndex;
}


mousse::vector mousse::conformalVoronoiMesh::calcSharedPatchNormal
(
  Cell_handle c1,
  Cell_handle c2
) const
{
  List<mousse::point> patchEdge{2, point::max};
  // Get shared Facet
  for (label cI = 0; cI < 4; ++cI) {
    if (c1->neighbor(cI) != c2 && !c1->vertex(cI)->constrained()) {
      if (c1->vertex(cI)->internalBoundaryPoint()) {
        patchEdge[0] = topoint(c1->vertex(cI)->point());
      } else {
        patchEdge[1] = topoint(c1->vertex(cI)->point());
      }
    }
  }
  Info << "    " << patchEdge << endl;
  return vector(patchEdge[1] - patchEdge[0]);
}


bool mousse::conformalVoronoiMesh::boundaryDualFace
(
  Cell_handle c1,
  Cell_handle c2
) const
{
  label nInternal = 0;
  label nExternal = 0;
  for (label cI = 0; cI < 4; ++cI) {
    if (c1->neighbor(cI) != c2 && !c1->vertex(cI)->constrained()) {
      if (c1->vertex(cI)->internalBoundaryPoint()) {
        nInternal++;
      } else if (c1->vertex(cI)->externalBoundaryPoint()) {
        nExternal++;
      }
    }
  }
  Info << "in = " << nInternal << " out = " << nExternal << endl;
  return (nInternal == 1 && nExternal == 1);
}


void mousse::conformalVoronoiMesh::createFacesOwnerNeighbourAndPatches
(
  const pointField& pts,
  faceList& faces,
  labelList& owner,
  labelList& neighbour,
  wordList& patchNames,
  PtrList<dictionary>& patchDicts,
  labelListList& patchPointPairSlaves,
  PackedBoolList& boundaryFacesToRemove,
  bool /*includeEmptyPatches*/
) const
{
  const label defaultPatchIndex = createPatchInfo(patchNames, patchDicts);
  const label nPatches = patchNames.size();
  labelList procNeighbours(nPatches, label(-1));
  FOR_ALL(procNeighbours, patchI) {
    if (patchDicts[patchI].found("neighbProcNo")) {
      procNeighbours[patchI] =
      (
        patchDicts[patchI].found("neighbProcNo")
        ? readLabel(patchDicts[patchI].lookup("neighbProcNo"))
        : -1
      );
    }
  }
  List<DynamicList<face>> patchFaces{nPatches, DynamicList<face>{0}};
  List<DynamicList<label>> patchOwners{nPatches, DynamicList<label>{0}};
  // Per patch face the index of the slave node of the point pair
  List<DynamicList<label>> patchPPSlaves{nPatches, DynamicList<label>{0}};
  List<DynamicList<bool>> indirectPatchFace{nPatches, DynamicList<bool>{0}};
  faces.setSize(number_of_finite_edges());
  owner.setSize(number_of_finite_edges());
  neighbour.setSize(number_of_finite_edges());
  boundaryFacesToRemove.setSize(number_of_finite_edges(), false);
  labelPairPairDynListList procPatchSortingIndex{nPatches};
  label dualFaceI = 0;
  if (foamyHexMeshControls().guardFeaturePoints()) {
    OBJstream startCellStr{"startingCell.obj"};
    OBJstream featurePointFacesStr{"ftPtFaces.obj"};
    OBJstream featurePointDualsStr{"ftPtDuals.obj"};
    OFstream cellStr{"vertexCells.obj"};
    label vcount = 1;
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         ++vit) {
      if (!vit->constrained())
        continue;
      // Find a starting cell
      std::list<Cell_handle> vertexCells;
      finite_incident_cells(vit, std::back_inserter(vertexCells));
      Cell_handle startCell;
      for (auto vcit = vertexCells.begin();
           vcit != vertexCells.end();
           ++vcit) {
        if ((*vcit)->featurePointExternalCell()) {
          startCell = *vcit;
        }
        if ((*vcit)->real()) {
          featurePointDualsStr.write
          (
            linePointRef(topoint(vit->point()), (*vcit)->dual())
          );
        }
      }
      // Error if startCell is null
      if (startCell == nullptr) {
        Pout << "Start cell is null!" << endl;
      }
      // Need to pick a direction to walk in
      Cell_handle vc1 = startCell;
      Cell_handle vc2;
      Info << "c1 index = " << vc1->cellIndex() << " "
        << vc1->dual() << endl;
      for (label cI = 0; cI < 4; ++cI) {
        Info << "c1 = " << cI << " "
          << vc1->neighbor(cI)->cellIndex() << " v = "
          << vc1->neighbor(cI)->dual() << endl;
        Info << vc1->vertex(cI)->info();
      }
      Cell_handle nextCell;
      for (label cI = 0; cI < 4; ++cI) {
        if (vc1->vertex(cI)->externalBoundaryPoint()) {
          vc2 = vc1->neighbor(cI);
          Info << "    c2 is neighbor "
            << vc2->cellIndex()
            << " of c1" << endl;
          for (label cI = 0; cI < 4; ++cI) {
            Info << "    c2 = " << cI << " "
              << vc2->neighbor(cI)->cellIndex() << " v = "
              << vc2->vertex(cI)->index() << endl;
          }
          face f{3};
          f[0] = vit->index();
          f[1] = vc1->cellIndex();
          f[2] = vc2->cellIndex();
          Info << "f " << f << endl;
          FOR_ALL(f, pI) {
            Info << "    " << pts[f[pI]] << endl;
          }
          vector correctNormal = calcSharedPatchNormal(vc1, vc2);
          correctNormal /= mag(correctNormal);
          Info << "    cN " << correctNormal << endl;
          vector fN = f.normal(pts);
          if (mag(fN) < SMALL) {
            nextCell = vc2;
            continue;
          }
          fN /= mag(fN);
          Info << "    fN " << fN << endl;
          if ((fN & correctNormal) > 0) {
            nextCell = vc2;
            break;
          }
        }
      }
      vc2 = nextCell;
      label own = vit->index();
      face f{3};
      f[0] = own;
      Info << "Start walk from " << vc1->cellIndex()
        << " to " << vc2->cellIndex() << endl;
      // Walk while not at start cell
      label iter = 0;
      do {
        Info << "     Walk from " << vc1->cellIndex()
          << " " << vc1->dual()
          << " to " << vc2->cellIndex()
          << " " << vc2->dual()
          << endl;
        startCellStr.write(linePointRef(vc1->dual(), vc2->dual()));
        // Get patch by getting face between cells and the two
        // points on the face that are not the feature vertex
        label patchIndex =
          geometryToConformTo_.findPatch
          (
            topoint(vit->point())
          );
        f[1] = vc1->cellIndex();
        f[2] = vc2->cellIndex();
        patchFaces[patchIndex].append(f);
        patchOwners[patchIndex].append(own);
        patchPPSlaves[patchIndex].append(own);
        // Find next cell
        Cell_handle nextCell;
        Info << "    c1 vertices " << vc2->dual() << endl;
        for (label cI = 0; cI < 4; ++cI) {
          Info << "        " << vc2->vertex(cI)->info();
        }
        Info << "    c1 neighbour vertices " << endl;
        for (label cI = 0; cI < 4; ++cI) {
          if (!vc2->vertex(cI)->constrained()
              && vc2->neighbor(cI) != vc1
              && !is_infinite(vc2->neighbor(cI))
              && (vc2->neighbor(cI)->featurePointExternalCell()
                  || vc2->neighbor(cI)->featurePointInternalCell())
              && vc2->neighbor(cI)->hasConstrainedPoint()) {
            DelaunayMeshTools::drawDelaunayCell
              (
                cellStr,
                vc2->neighbor(cI),
                vcount++
              );
            Info << "        neighbour " << cI << " "
              << vc2->neighbor(cI)->dual() << endl;
            for (label I = 0; I < 4; ++I) {
              Info << "            "
                << vc2->neighbor(cI)->vertex(I)->info();
            }
          }
        }
        for (label cI = 0; cI < 4; ++cI) {
          if (!vc2->vertex(cI)->constrained()
              && vc2->neighbor(cI) != vc1
              && !is_infinite(vc2->neighbor(cI))
              && (vc2->neighbor(cI)->featurePointExternalCell()
                  || vc2->neighbor(cI)->featurePointInternalCell())
              && vc2->neighbor(cI)->hasConstrainedPoint()) {
            // check if shared edge is internal/internal
            if (boundaryDualFace(vc2, vc2->neighbor(cI))) {
              nextCell = vc2->neighbor(cI);
              break;
            }
          }
        }
        vc1 = vc2;
        vc2 = nextCell;
        iter++;
      } while (vc1 != startCell && iter < 100);
    }
  }
  for (auto eit = finite_edges_begin();
       eit != finite_edges_end();
       ++eit) {
    Cell_handle c = eit->first;
    Vertex_handle vA = c->vertex(eit->second);
    Vertex_handle vB = c->vertex(eit->third);
    if (vA->constrained() && vB->constrained()) {
      continue;
    }
    if ((vA->constrained() && vB->internalOrBoundaryPoint())
        || (vB->constrained() && vA->internalOrBoundaryPoint())) {
      face newDualFace = buildDualFace(eit);
      label own = -1;
      label nei = -1;
      if (ownerAndNeighbour(vA, vB, own, nei)) {
        reverse(newDualFace);
      }
      // internal face
      faces[dualFaceI] = newDualFace;
      owner[dualFaceI] = own;
      neighbour[dualFaceI] = nei;
      dualFaceI++;
    } else if ((vA->internalOrBoundaryPoint() && !vA->referred())
               || (vB->internalOrBoundaryPoint() && !vB->referred())) {
      if ((vA->internalPoint() && vB->externalBoundaryPoint())
          || (vB->internalPoint() && vA->externalBoundaryPoint())) {
        Cell_circulator ccStart = incident_cells(*eit);
        Cell_circulator cc1 = ccStart;
        Cell_circulator cc2 = cc1;
        cc2++;
        bool skipEdge = false;
        do {
          if (cc1->hasFarPoint() || cc2->hasFarPoint()
              || is_infinite(cc1) || is_infinite(cc2)) {
            Pout << "Ignoring edge between internal and external: "
              << vA->info() << vB->info();
            skipEdge = true;
            break;
          }
          cc1++;
          cc2++;
        } while (cc1 != ccStart);
        // Do not create faces if the internal point is outside!
        // This occurs because the internal point is not determined to
        // be outside in the inside/outside test. This is most likely
        // due to the triangle.nearestPointClassify test not returning
        // edge/point as the nearest type.
        if (skipEdge) {
          continue;
        }
      }
      face newDualFace = buildDualFace(eit);
      if (newDualFace.size() >= 3) {
        label own = -1;
        label nei = -1;
        if (ownerAndNeighbour(vA, vB, own, nei)) {
          reverse(newDualFace);
        }
        label patchIndex = -1;
        pointFromPoint ptA = topoint(vA->point());
        pointFromPoint ptB = topoint(vB->point());
        if (nei == -1) {
          // boundary face
          if (isProcBoundaryEdge(eit)) {
            // One (and only one) of the points is an internal
            // point from another processor
            label procIndex = max(vA->procIndex(), vB->procIndex());
            patchIndex =
              max
              (
                findIndex(procNeighbours, vA->procIndex()),
                findIndex(procNeighbours, vB->procIndex())
              );
            // The lower processor index is the owner of the
            // two for the purpose of sorting the patch faces.
            if (Pstream::myProcNo() < procIndex) {
              // Use this processor's vertex index as the master
              // for sorting
              DynamicList<Pair<labelPair>>& sortingIndex =
                procPatchSortingIndex[patchIndex];
              if (vB->internalOrBoundaryPoint() && vB->referred()) {
                sortingIndex.append
                (
                  Pair<labelPair>
                  {
                    labelPair(vA->index(), vA->procIndex()),
                    labelPair(vB->index(), vB->procIndex())
                  }
                );
              } else {
                sortingIndex.append
                (
                  Pair<labelPair>
                  {
                    labelPair(vB->index(), vB->procIndex()),
                    labelPair(vA->index(), vA->procIndex())
                  }
                );
              }
            } else {
              // Use the other processor's vertex index as the
              // master for sorting
              DynamicList<Pair<labelPair>>& sortingIndex =
                procPatchSortingIndex[patchIndex];
              if (vA->internalOrBoundaryPoint() && vA->referred()) {
                sortingIndex.append
                (
                  Pair<labelPair>
                  {
                    labelPair(vA->index(), vA->procIndex()),
                    labelPair(vB->index(), vB->procIndex())
                  }
                );
              } else {
                sortingIndex.append
                (
                  Pair<labelPair>
                  {
                    labelPair(vB->index(), vB->procIndex()),
                    labelPair(vA->index(), vA->procIndex())
                  }
                );
              }
            }
          } else {
            patchIndex = geometryToConformTo_.findPatch(ptA, ptB);
          }
          if (patchIndex == -1)
          {
            // Did not find a surface patch between
            // between Dv pair, finding nearest patch
            patchIndex =
              geometryToConformTo_.findPatch
              (
                0.5*(ptA + ptB)
              );
          }
          patchFaces[patchIndex].append(newDualFace);
          patchOwners[patchIndex].append(own);
          // If the two vertices are a pair, then the patch face is
          // a desired one.
          if (vA->boundaryPoint() && vB->boundaryPoint()
              && !ptPairs_.isPointPair(vA, vB)
              && !ftPtConformer_.featurePointPairs().isPointPair(vA, vB)) {
            indirectPatchFace[patchIndex].append(true);
          } else {
            indirectPatchFace[patchIndex].append(false);
          }
          // Store the non-internal or boundary point
          if (vA->internalOrBoundaryPoint()) {
            patchPPSlaves[patchIndex].append(vB->index());
          } else {
            patchPPSlaves[patchIndex].append(vA->index());
          }
        } else {
          patchIndex = geometryToConformTo_.findPatch(ptA, ptB);
          // internal face
          faces[dualFaceI] = newDualFace;
          owner[dualFaceI] = own;
          neighbour[dualFaceI] = nei;
          dualFaceI++;
        }
      }
    }
  }
  if (!patchFaces[defaultPatchIndex].empty()) {
    Pout << nl << patchFaces[defaultPatchIndex].size()
      << " faces were not able to have their patch determined from "
      << "the surface. "
      << nl <<  "Adding to patch " << patchNames[defaultPatchIndex]
      << endl;
  }
  label nInternalFaces = dualFaceI;
  faces.setSize(nInternalFaces);
  owner.setSize(nInternalFaces);
  neighbour.setSize(nInternalFaces);
  timeCheck("polyMesh quality checked");
  sortFaces(faces, owner, neighbour);
  sortProcPatches
    (
      patchFaces,
      patchOwners,
      patchPPSlaves,
      procPatchSortingIndex
    );
  timeCheck("faces, owner, neighbour sorted");
  addPatches
    (
      nInternalFaces,
      faces,
      owner,
      patchDicts,
      boundaryFacesToRemove,
      patchFaces,
      patchOwners,
      indirectPatchFace
    );
  // Return     patchPointPairSlaves.setSize(nPatches);
  patchPointPairSlaves.setSize(nPatches);
  FOR_ALL(patchPPSlaves, patchI) {
    patchPointPairSlaves[patchI].transfer(patchPPSlaves[patchI]);
  }
  if (foamyHexMeshControls().objOutput()) {
    Info << "Writing processor interfaces" << endl;
    FOR_ALL(patchDicts, nbI) {
      if (patchFaces[nbI].size() > 0) {
        const label neighbour =
        (
          patchDicts[nbI].found("neighbProcNo")
          ? readLabel(patchDicts[nbI].lookup("neighbProcNo"))
          : -1
        );
        faceList procPatchFaces = patchFaces[nbI];
        // Reverse faces as it makes it easier to analyse the output
        // using a diff
        if (neighbour < Pstream::myProcNo()) {
          FOR_ALL(procPatchFaces, fI) {
            procPatchFaces[fI] = procPatchFaces[fI].reverseFace();
          }
        }
        if (neighbour != -1) {
          word fName =
            "processor_" + name(Pstream::myProcNo()) + "_to_" + name(neighbour)
            + "_interface.obj";
          DelaunayMeshTools::writeProcessorInterface
          (
            time().path()/fName,
            *this,
            procPatchFaces
          );
        }
      }
    }
  }
}


void mousse::conformalVoronoiMesh::sortFaces
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


void mousse::conformalVoronoiMesh::sortProcPatches
(
  List<DynamicList<face>>& patchFaces,
  List<DynamicList<label>>& patchOwners,
  List<DynamicList<label>>& patchPointPairSlaves,
  labelPairPairDynListList& patchSortingIndices
) const
{
  if (!Pstream::parRun()) {
    return;
  }
  FOR_ALL(patchSortingIndices, patchI) {
    faceList& faces = patchFaces[patchI];
    labelList& owner = patchOwners[patchI];
    DynamicList<label>& slaves = patchPointPairSlaves[patchI];
    DynamicList<Pair<labelPair>>& sortingIndices
      = patchSortingIndices[patchI];
    if (sortingIndices.empty())
      continue;
    if (faces.size() != sortingIndices.size()
        || owner.size() != sortingIndices.size()
        || slaves.size() != sortingIndices.size()) {
      FATAL_ERROR_IN
      (
        "void mousse::conformalVoronoiMesh::sortProcPatches"
        "("
        "  List<DynamicList<face>>& patchFaces, "
        "  List<DynamicList<label>>& patchOwners, "
        "  const List<DynamicList<label>>& patchSortingIndices"
        ") const"
      )
      << "patch size and size of sorting indices is inconsistent "
      << " for patch " << patchI << nl
      << " faces.size() " << faces.size() << nl
      << " owner.size() " << owner.size() << nl
      << " slaves.size() " << slaves.size() << nl
      << " sortingIndices.size() "
      << sortingIndices.size()
      << exit(FatalError) << endl;
    }
    labelList oldToNew;
    sortedOrder(sortingIndices, oldToNew);
    oldToNew = invert(oldToNew.size(), oldToNew);
    inplaceReorder(oldToNew, sortingIndices);
    inplaceReorder(oldToNew, faces);
    inplaceReorder(oldToNew, owner);
    inplaceReorder(oldToNew, slaves);
  }
}


void mousse::conformalVoronoiMesh::addPatches
(
  const label nInternalFaces,
  faceList& faces,
  labelList& owner,
  PtrList<dictionary>& patchDicts,
  PackedBoolList& boundaryFacesToRemove,
  const List<DynamicList<face>>& patchFaces,
  const List<DynamicList<label>>& patchOwners,
  const List<DynamicList<bool>>& indirectPatchFace
) const
{
  label nBoundaryFaces = 0;
  FOR_ALL(patchFaces, p) {
    patchDicts[p].set("nFaces", patchFaces[p].size());
    patchDicts[p].set("startFace", nInternalFaces + nBoundaryFaces);
    nBoundaryFaces += patchFaces[p].size();
  }
  faces.setSize(nInternalFaces + nBoundaryFaces);
  owner.setSize(nInternalFaces + nBoundaryFaces);
  boundaryFacesToRemove.setSize(nInternalFaces + nBoundaryFaces);
  label faceI = nInternalFaces;
  FOR_ALL(patchFaces, p) {
    FOR_ALL(patchFaces[p], f) {
      faces[faceI] = patchFaces[p][f];
      owner[faceI] = patchOwners[p][f];
      boundaryFacesToRemove[faceI] = indirectPatchFace[p][f];
      faceI++;
    }
  }
}


void mousse::conformalVoronoiMesh::removeUnusedPoints
(
  faceList& faces,
  pointField& pts,
  labelList& boundaryPts
) const
{
  Info << nl << "Removing unused points" << endl;
  PackedBoolList ptUsed{pts.size(), false};
  // Scan all faces to find all of the points that are used
  FOR_ALL(faces, fI) {
    const face& f = faces[fI];
    FOR_ALL(f, fPtI) {
      ptUsed[f[fPtI]] = true;
    }
  }
  label pointI = 0;
  labelList oldToNew{pts.size(), label(-1)};
  // Move all of the used points to the start of the pointField and
  // truncate it
  FOR_ALL(ptUsed, ptUI) {
    if (ptUsed[ptUI] == true) {
      oldToNew[ptUI] = pointI++;
    }
  }
  inplaceReorder(oldToNew, pts);
  inplaceReorder(oldToNew, boundaryPts);
  Info << "    Removing "
    << returnReduce(pts.size() - pointI, sumOp<label>())
    << " unused points"
    << endl;
  pts.setSize(pointI);
  boundaryPts.setSize(pointI);
  // Renumber the faces to use the new point numbers
  FOR_ALL(faces, fI) {
    inplaceRenumber(oldToNew, faces[fI]);
  }
}


mousse::labelList mousse::conformalVoronoiMesh::removeUnusedCells
(
  labelList& owner,
  labelList& neighbour
) const
{
  Info << nl << "Removing unused cells" << endl;
  PackedBoolList cellUsed{vertexCount(), false};
  // Scan all faces to find all of the cells that are used
  FOR_ALL(owner, oI) {
    cellUsed[owner[oI]] = true;
  }
  FOR_ALL(neighbour, nI) {
    cellUsed[neighbour[nI]] = true;
  }
  label cellI = 0;
  labelList oldToNew{cellUsed.size(), label(-1)};
  // Move all of the used cellCentres to the start of the pointField and
  // truncate it
  FOR_ALL(cellUsed, cellUI) {
    if (cellUsed[cellUI] == true) {
      oldToNew[cellUI] = cellI++;
    }
  }
  labelList newToOld{invert(cellI, oldToNew)};
  // Find all of the unused cells, create a list of them, then
  // subtract one from each owner and neighbour entry for each of
  // the unused cell indices that it is above.
  DynamicList<label> unusedCells;
  FOR_ALL(cellUsed, cUI) {
    if (cellUsed[cUI] == false) {
      unusedCells.append(cUI);
    }
  }
  if (unusedCells.size() > 0) {
    Info << "    Removing "
      << returnReduce(unusedCells.size(), sumOp<label>())
      <<  " unused cell labels" << endl;
    FOR_ALL(owner, oI) {
      label& o = owner[oI];
      o -= findLower(unusedCells, o) + 1;
    }
    FOR_ALL(neighbour, nI) {
      label& n = neighbour[nI];
      n -= findLower(unusedCells, n) + 1;
    }
  }
  return newToOld;
}

