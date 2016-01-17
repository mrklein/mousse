// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conformal_voronoi_mesh.hpp"
#include "iostreams.hpp"
#include "ofstream.hpp"
#include "point_mesh.hpp"
#include "point_fields.hpp"
#include "list_ops.hpp"
#include "poly_mesh_filter.hpp"
#include "poly_topo_change.hpp"
#include "print_table.hpp"
#include "point_mesh.hpp"
#include "indexed_vertex_ops.hpp"
#include "delaunay_mesh_tools.hpp"
#include "sync_tools.hpp"
#include "face_set.hpp"
#include "obj_stream.hpp"
// Member Functions 
void mousse::conformalVoronoiMesh::timeCheck
(
  const string& description
) const
{
  timeCheck(time(), description, foamyHexMeshControls().timeChecks());
}
void mousse::conformalVoronoiMesh::timeCheck
(
  const Time& runTime,
  const string& description,
  const bool check
)
{
  if (check)
  {
    Info<< nl << "--- [ cpuTime "
      << runTime.elapsedCpuTime() << " s, "
      << "delta " << runTime.cpuTimeIncrement()<< " s";
    if (description != word::null)
    {
      Info<< ", " << description << " ";
    }
    else
    {
      Info<< " ";
    }
    Info<< "] --- " << endl;
    memInfo m;
    if (m.valid())
    {
      PrintTable<word, label> memoryTable
      (
        "Memory Usage (kB): "
       + description
      );
      memoryTable.add("mSize", m.size());
      memoryTable.add("mPeak", m.peak());
      memoryTable.add("mRss", m.rss());
      Info<< incrIndent;
      memoryTable.print(Info, true, true);
      Info<< decrIndent;
    }
  }
}
void mousse::conformalVoronoiMesh::writeMesh(const fileName& instance)
{
  DelaunayMeshTools::writeInternalDelaunayVertices(instance, *this);
  // Per cell the Delaunay vertex
  labelList cellToDelaunayVertex;
  // Per patch, per face the Delaunay vertex
  labelListList patchToDelaunayVertex;
  labelList dualPatchStarts;
  {
    pointField points;
    labelList boundaryPts;
    faceList faces;
    labelList owner;
    labelList neighbour;
    wordList patchNames;
    PtrList<dictionary> patchDicts;
    pointField cellCentres;
    PackedBoolList boundaryFacesToRemove;
    calcDualMesh
    (
      points,
      boundaryPts,
      faces,
      owner,
      neighbour,
      patchNames,
      patchDicts,
      cellCentres,
      cellToDelaunayVertex,
      patchToDelaunayVertex,
      boundaryFacesToRemove
    );
    Info<< nl << "Writing polyMesh to " << instance << endl;
    writeMesh
    (
      mousse::polyMesh::defaultRegion,
      instance,
      points,
      boundaryPts,
      faces,
      owner,
      neighbour,
      patchNames,
      patchDicts,
      cellCentres,
      boundaryFacesToRemove
    );
    dualPatchStarts.setSize(patchDicts.size());
    forAll(dualPatchStarts, patchI)
    {
      dualPatchStarts[patchI] =
        readLabel(patchDicts[patchI].lookup("startFace"));
    }
  }
  if (foamyHexMeshControls().writeCellShapeControlMesh())
  {
    cellShapeControls().shapeControlMesh().write();
  }
  if (foamyHexMeshControls().writeBackgroundMeshDecomposition())
  {
    Info<< nl << "Writing " << "backgroundMeshDecomposition" << endl;
    // Have to explicitly update the mesh instance.
    const_cast<fvMesh&>(decomposition_().mesh()).setInstance
    (
      time().timeName()
    );
    decomposition_().mesh().write();
  }
  if (foamyHexMeshControls().writeTetDualMesh())
  {
    label cellI = 0;
    for
    (
      Finite_cells_iterator cit = finite_cells_begin();
      cit != finite_cells_end();
      ++cit
    )
    {
      if
      (
        !cit->hasFarPoint()
      && !is_infinite(cit)
      )
      {
        cit->cellIndex() = cellI++;
      }
    }
    Info<< nl << "Writing " << "tetDualMesh" << endl;
    DistributedDelaunayMesh<Delaunay>::labelTolabelPairHashTable vertexMap;
    labelList cellMap;
    autoPtr<polyMesh> tetMesh =
      createMesh("tetDualMesh", vertexMap, cellMap);
    tetMesh().write();
//        // Determine map from Delaunay vertex to Dual mesh
//        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        // From all Delaunay vertices to cell (positive index)
//        // or patch face (negative index)
//        labelList vertexToDualAddressing(number_of_vertices(), 0);
//
//        forAll(cellToDelaunayVertex, cellI)
//        {
//            label vertI = cellToDelaunayVertex[cellI];
//
//            if (vertexToDualAddressing[vertI] != 0)
//            {
//                FatalErrorIn("conformalVoronoiMesh::writeMesh(..)")
//                    << "Delaunay vertex " << vertI
//                    << " from cell " << cellI
//                    << " is already mapped to "
//                    << vertexToDualAddressing[vertI]
//                    << exit(FatalError);
//            }
//            vertexToDualAddressing[vertI] = cellI+1;
//        }
//
//        forAll(patchToDelaunayVertex, patchI)
//        {
//            const labelList& patchVertices = patchToDelaunayVertex[patchI];
//
//            forAll(patchVertices, i)
//            {
//                label vertI = patchVertices[i];
//
//                if (vertexToDualAddressing[vertI] > 0)
//                {
//                    FatalErrorIn("conformalVoronoiMesh::writeMesh(..)")
//                        << "Delaunay vertex " << vertI
//                        << " from patch " << patchI
//                        << " local index " << i
//                        << " is already mapped to cell "
//                        << vertexToDualAddressing[vertI]-1
//                        << exit(FatalError);
//                }
//
//                // Vertex might be used by multiple faces. Which one to
//                // use? For now last one wins.
//                label dualFaceI = dualPatchStarts[patchI]+i;
//                vertexToDualAddressing[vertI] = -dualFaceI-1;
//            }
//        }
//
//
//        // Calculate tet mesh addressing
//        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        pointField points;
//        labelList boundaryPts(number_of_finite_cells(), -1);
//        // From tet point back to Delaunay vertex index
//        labelList pointToDelaunayVertex;
//        faceList faces;
//        labelList owner;
//        labelList neighbour;
//        wordList patchTypes;
//        wordList patchNames;
//        PtrList<dictionary> patchDicts;
//        pointField cellCentres;
//
//        calcTetMesh
//        (
//            points,
//            pointToDelaunayVertex,
//            faces,
//            owner,
//            neighbour,
//            patchTypes,
//            patchNames,
//            patchDicts
//        );
//
//
//
//        // Calculate map from tet points to dual mesh cells/patch faces
//        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        labelIOList pointDualAddressing
//        (
//            IOobject
//            (
//                "pointDualAddressing",
//                instance,
//                "tetDualMesh"/polyMesh::meshSubDir,
//                runTime_,
//                IOobject::NO_READ,
//                IOobject::AUTO_WRITE,
//                false
//            ),
//            UIndirectList<label>
//            (
//                vertexToDualAddressing,
//                pointToDelaunayVertex
//            )()
//        );
//
//        label pointI = findIndex(pointDualAddressing, -1);
//        if (pointI != -1)
//        {
//            WarningIn
//            (
//                "conformalVoronoiMesh::writeMesh\n"
//                "(\n"
//                "    const fileName& instance,\n"
//                "    bool filterFaces\n"
//                ")\n"
//            )   << "Delaunay vertex " << pointI
//                << " does not have a corresponding dual cell." << endl;
//        }
//
//        Info<< "Writing map from tetDualMesh points to Voronoi mesh to "
//            << pointDualAddressing.objectPath() << endl;
//        pointDualAddressing.write();
//
//
//
//        // Write tet points corresponding to the Voronoi cell/face centre
//        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        {
//            // Read Voronoi mesh
//            fvMesh mesh
//            (
//                IOobject
//                (
//                    mousse::polyMesh::defaultRegion,
//                    instance,
//                    runTime_,
//                    IOobject::MUST_READ
//                )
//            );
//            pointIOField dualPoints
//            (
//                IOobject
//                (
//                    "dualPoints",
//                    instance,
//                    "tetDualMesh"/polyMesh::meshSubDir,
//                    runTime_,
//                    IOobject::NO_READ,
//                    IOobject::AUTO_WRITE,
//                    false
//                ),
//                points
//            );
//
//            forAll(pointDualAddressing, pointI)
//            {
//                label index = pointDualAddressing[pointI];
//
//                if (index > 0)
//                {
//                    label cellI = index-1;
//                    dualPoints[pointI] = mesh.cellCentres()[cellI];
//                }
//                else if (index < 0)
//                {
//                    label faceI = -index-1;
//                    if (faceI >= mesh.nInternalFaces())
//                    {
//                        dualPoints[pointI] = mesh.faceCentres()[faceI];
//                    }
//                }
//            }
//
//            Info<< "Writing tetDualMesh points mapped onto Voronoi mesh to "
//                << dualPoints.objectPath() << endl
//                << "Replace the polyMesh/points with these." << endl;
//            dualPoints.write();
//        }
//
//
//        Info<< nl << "Writing tetDualMesh to " << instance << endl;
//
//        PackedBoolList boundaryFacesToRemove;
//        writeMesh
//        (
//            "tetDualMesh",
//            instance,
//            points,
//            boundaryPts,
//            faces,
//            owner,
//            neighbour,
//            patchTypes,
//            patchNames,
//            patchDicts,
//            cellCentres,
//            boundaryFacesToRemove
//        );
  }
}
mousse::autoPtr<mousse::fvMesh> mousse::conformalVoronoiMesh::createDummyMesh
(
  const IOobject& io,
  const wordList& patchNames,
  const PtrList<dictionary>& patchDicts
) const
{
  autoPtr<fvMesh> meshPtr
  (
    new fvMesh
    (
      io,
      xferCopy(pointField()),
      xferCopy(faceList()),
      xferCopy(cellList())
    )
  );
  fvMesh& mesh = meshPtr();
  List<polyPatch*> patches(patchDicts.size());
  forAll(patches, patchI)
  {
    if
    (
      patchDicts.set(patchI)
    && (
        word(patchDicts[patchI].lookup("type"))
      == processorPolyPatch::typeName
      )
    )
    {
      patches[patchI] = new processorPolyPatch
      (
        patchNames[patchI],
        0,          //patchSizes[p],
        0,          //patchStarts[p],
        patchI,
        mesh.boundaryMesh(),
        readLabel(patchDicts[patchI].lookup("myProcNo")),
        readLabel(patchDicts[patchI].lookup("neighbProcNo")),
        coupledPolyPatch::COINCIDENTFULLMATCH
      );
    }
    else
    {
      patches[patchI] = polyPatch::New
      (
        patchDicts[patchI].lookup("type"),
        patchNames[patchI],
        0,          //patchSizes[p],
        0,          //patchStarts[p],
        patchI,
        mesh.boundaryMesh()
      ).ptr();
    }
  }
  mesh.addFvPatches(patches);
  return meshPtr;
}
void mousse::conformalVoronoiMesh::checkProcessorPatchesMatch
(
  const PtrList<dictionary>& patchDicts
) const
{
  // Check patch sizes
  labelListList procPatchSizes
  (
    Pstream::nProcs(),
    labelList(Pstream::nProcs(), -1)
  );
  forAll(patchDicts, patchI)
  {
    if
    (
      patchDicts.set(patchI)
    && (
        word(patchDicts[patchI].lookup("type"))
      == processorPolyPatch::typeName
      )
    )
    {
      const label procNeighb =
        readLabel(patchDicts[patchI].lookup("neighbProcNo"));
      procPatchSizes[Pstream::myProcNo()][procNeighb]
        = readLabel(patchDicts[patchI].lookup("nFaces"));
    }
  }
  Pstream::gatherList(procPatchSizes);
  if (Pstream::master())
  {
    bool allMatch = true;
    forAll(procPatchSizes, procI)
    {
      const labelList& patchSizes = procPatchSizes[procI];
      forAll(patchSizes, patchI)
      {
        if (patchSizes[patchI] != procPatchSizes[patchI][procI])
        {
          allMatch = false;
          Info<< indent << "Patches " << procI << " and " << patchI
            << " have different sizes: " << patchSizes[patchI]
            << " and " << procPatchSizes[patchI][procI] << endl;
        }
      }
    }
    if (allMatch)
    {
      Info<< indent << "All processor patches have matching numbers of "
        << "faces" << endl;
    }
  }
}
void mousse::conformalVoronoiMesh::reorderPoints
(
  pointField& points,
  labelList& boundaryPts,
  faceList& faces,
  const label nInternalFaces
) const
{
  Info<< incrIndent << indent << "Reordering points into internal/external"
    << endl;
  labelList oldToNew(points.size(), label(0));
  // Find points that are internal
  for (label fI = nInternalFaces; fI < faces.size(); ++fI)
  {
    const face& f = faces[fI];
    forAll(f, fpI)
    {
      oldToNew[f[fpI]] = 1;
    }
  }
  const label nInternalPoints = points.size() - sum(oldToNew);
  label countInternal = 0;
  label countExternal = nInternalPoints;
  forAll(points, pI)
  {
    if (oldToNew[pI] == 0)
    {
      oldToNew[pI] = countInternal++;
    }
    else
    {
      oldToNew[pI] = countExternal++;
    }
  }
  Info<< indent
    << "Number of internal points: " << countInternal << nl
    << indent << "Number of external points: " << countExternal
    << decrIndent << endl;
  inplaceReorder(oldToNew, points);
  inplaceReorder(oldToNew, boundaryPts);
  forAll(faces, fI)
  {
    face& f = faces[fI];
    forAll(f, fpI)
    {
      f[fpI] = oldToNew[f[fpI]];
    }
  }
}
void mousse::conformalVoronoiMesh::reorderProcessorPatches
(
  const word& meshName,
  const fileName& instance,
  const pointField& points,
  faceList& faces,
  const wordList& patchNames,
  const PtrList<dictionary>& patchDicts
) const
{
  Info<< incrIndent << indent << "Reordering processor patches" << endl;
  Info<< incrIndent;
  checkProcessorPatchesMatch(patchDicts);
  // Create dummy mesh with correct proc boundaries to do sorting
  autoPtr<fvMesh> sortMeshPtr
  (
    createDummyMesh
    (
      IOobject
      (
        meshName,
        instance,
        runTime_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      patchNames,
      patchDicts
    )
  );
  const fvMesh& sortMesh = sortMeshPtr();
  // Change the transform type on processors to coincident full match.
//    forAll(sortMesh.boundaryMesh(), patchI)
//    {
//        const polyPatch& patch = sortMesh.boundaryMesh()[patchI];
//
//        if (isA<processorPolyPatch>(patch))
//        {
//            const processorPolyPatch& cpPatch
//                = refCast<const processorPolyPatch>(patch);
//
//            processorPolyPatch& pPatch
//                = const_cast<processorPolyPatch&>(cpPatch);
//
//            pPatch.transform() = coupledPolyPatch::COINCIDENTFULLMATCH;
//        }
//    }
  // Rotation on new faces.
  labelList rotation(faces.size(), label(0));
  labelList faceMap(faces.size(), label(-1));
  PstreamBuffers pBufs(Pstream::nonBlocking);
  // Send ordering
  forAll(sortMesh.boundaryMesh(), patchI)
  {
    const polyPatch& pp = sortMesh.boundaryMesh()[patchI];
    if (isA<processorPolyPatch>(pp))
    {
      refCast<const processorPolyPatch>(pp).initOrder
      (
        pBufs,
        primitivePatch
        (
          SubList<face>
          (
            faces,
            readLabel(patchDicts[patchI].lookup("nFaces")),
            readLabel(patchDicts[patchI].lookup("startFace"))
          ),
          points
        )
      );
    }
  }
  pBufs.finishedSends();
  Info<< incrIndent << indent << "Face ordering initialised..." << endl;
  // Receive and calculate ordering
  bool anyChanged = false;
  forAll(sortMesh.boundaryMesh(), patchI)
  {
    const polyPatch& pp = sortMesh.boundaryMesh()[patchI];
    if (isA<processorPolyPatch>(pp))
    {
      const label nPatchFaces =
        readLabel(patchDicts[patchI].lookup("nFaces"));
      const label patchStartFace =
        readLabel(patchDicts[patchI].lookup("startFace"));
      labelList patchFaceMap(nPatchFaces, label(-1));
      labelList patchFaceRotation(nPatchFaces, label(0));
      bool changed = refCast<const processorPolyPatch>(pp).order
      (
        pBufs,
        primitivePatch
        (
          SubList<face>
          (
            faces,
            nPatchFaces,
            patchStartFace
          ),
          points
        ),
        patchFaceMap,
        patchFaceRotation
      );
      if (changed)
      {
        // Merge patch face reordering into mesh face reordering table
        forAll(patchFaceRotation, patchFaceI)
        {
          rotation[patchFaceI + patchStartFace]
            = patchFaceRotation[patchFaceI];
        }
        forAll(patchFaceMap, patchFaceI)
        {
          if (patchFaceMap[patchFaceI] != patchFaceI)
          {
            faceMap[patchFaceI + patchStartFace]
              = patchFaceMap[patchFaceI] + patchStartFace;
          }
        }
        anyChanged = true;
      }
    }
  }
  Info<< incrIndent << indent << "Faces matched." << endl;
  reduce(anyChanged, orOp<bool>());
  if (anyChanged)
  {
    label nReorderedFaces = 0;
    forAll(faceMap, faceI)
    {
     if (faceMap[faceI] != -1)
     {
       nReorderedFaces++;
     }
    }
    if (nReorderedFaces > 0)
    {
      inplaceReorder(faceMap, faces);
    }
    // Rotate faces (rotation is already in new face indices).
    label nRotated = 0;
    forAll(rotation, faceI)
    {
      if (rotation[faceI] != 0)
      {
        faces[faceI] = rotateList(faces[faceI], rotation[faceI]);
        nRotated++;
      }
    }
    Info<< indent << returnReduce(nReorderedFaces, sumOp<label>())
      << " faces have been reordered" << nl
      << indent << returnReduce(nRotated, sumOp<label>())
      << " faces have been rotated"
      << decrIndent << decrIndent
      << decrIndent << decrIndent << endl;
  }
}
void mousse::conformalVoronoiMesh::writeMesh
(
  const word& meshName,
  const fileName& instance,
  pointField& points,
  labelList& boundaryPts,
  faceList& faces,
  labelList& owner,
  labelList& neighbour,
  const wordList& patchNames,
  const PtrList<dictionary>& patchDicts,
  const pointField& cellCentres,
  PackedBoolList& boundaryFacesToRemove
) const
{
  if (foamyHexMeshControls().objOutput())
  {
    DelaunayMeshTools::writeObjMesh
    (
      time().path()/word(meshName + ".obj"),
      points,
      faces
    );
  }
  const label nInternalFaces = readLabel(patchDicts[0].lookup("startFace"));
  reorderPoints(points, boundaryPts, faces, nInternalFaces);
  if (Pstream::parRun())
  {
    reorderProcessorPatches
    (
      meshName,
      instance,
      points,
      faces,
      patchNames,
      patchDicts
    );
  }
  Info<< incrIndent;
  Info<< indent << "Constructing mesh" << endl;
  timeCheck("Before fvMesh construction");
  fvMesh mesh
  (
    IOobject
    (
      meshName,
      instance,
      runTime_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    ),
    xferMove(points),
    xferMove(faces),
    xferMove(owner),
    xferMove(neighbour)
  );
  Info<< indent << "Adding patches to mesh" << endl;
  List<polyPatch*> patches(patchNames.size());
  label nValidPatches = 0;
  forAll(patches, p)
  {
    label totalPatchSize = readLabel(patchDicts[p].lookup("nFaces"));
    if
    (
      patchDicts.set(p)
    && (
        word(patchDicts[p].lookup("type"))
      == processorPolyPatch::typeName
      )
    )
    {
      const_cast<dictionary&>(patchDicts[p]).set
      (
        "transform",
        "noOrdering"
        //"coincidentFullMatch"
      );
      // Do not create empty processor patches
      if (totalPatchSize > 0)
      {
        patches[nValidPatches] = new processorPolyPatch
        (
          patchNames[p],
          patchDicts[p],
          nValidPatches,
          mesh.boundaryMesh(),
          processorPolyPatch::typeName
        );
        nValidPatches++;
      }
    }
    else
    {
      // Check that the patch is not empty on every processor
      reduce(totalPatchSize, sumOp<label>());
      if (totalPatchSize > 0)
      {
        patches[nValidPatches] = polyPatch::New
        (
          patchNames[p],
          patchDicts[p],
          nValidPatches,
          mesh.boundaryMesh()
        ).ptr();
        nValidPatches++;
      }
    }
  }
  patches.setSize(nValidPatches);
  mesh.addFvPatches(patches);
  // Add zones to the mesh
  addZones(mesh, cellCentres);
  Info<< indent << "Add pointZones" << endl;
  {
    label sz = mesh.pointZones().size();
    DynamicList<label> bPts(boundaryPts.size());
    forAll(dualMeshPointTypeNames_, typeI)
    {
      forAll(boundaryPts, ptI)
      {
        const label& bPtType = boundaryPts[ptI];
        if (bPtType == typeI)
        {
          bPts.append(ptI);
        }
      }
//            syncTools::syncPointList(mesh, bPts, maxEqOp<label>(), -1);
      Info<< incrIndent << indent
        << "Adding " << bPts.size()
        << " points of type " << dualMeshPointTypeNames_.words()[typeI]
        << decrIndent << endl;
      mesh.pointZones().append
      (
        new pointZone
        (
          dualMeshPointTypeNames_.words()[typeI],
          bPts,
          sz + typeI,
          mesh.pointZones()
        )
      );
      bPts.clear();
    }
  }
  // Add indirectPatchFaces to a face zone
  Info<< indent << "Adding indirect patch faces set" << endl;
  syncTools::syncFaceList
  (
    mesh,
    boundaryFacesToRemove,
    orEqOp<unsigned int>()
  );
  labelList addr(boundaryFacesToRemove.count());
  label count = 0;
  forAll(boundaryFacesToRemove, faceI)
  {
    if (boundaryFacesToRemove[faceI])
    {
      addr[count++] = faceI;
    }
  }
  addr.setSize(count);
  faceSet indirectPatchFaces
  (
    mesh,
    "indirectPatchFaces",
    addr,
    IOobject::AUTO_WRITE
  );
  indirectPatchFaces.sync(mesh);
  Info<< decrIndent;
  timeCheck("Before fvMesh filtering");
  autoPtr<polyMeshFilter> meshFilter;
  label nInitialBadFaces = 0;
  if (foamyHexMeshControls().filterEdges())
  {
    Info<< nl << "Filtering edges on polyMesh" << nl << endl;
    meshFilter.reset(new polyMeshFilter(mesh, boundaryPts));
    // Filter small edges only. This reduces the number of faces so that
    // the face filtering is sped up.
    nInitialBadFaces = meshFilter().filterEdges(0);
    {
      const autoPtr<fvMesh>& newMesh = meshFilter().filteredMesh();
      polyTopoChange meshMod(newMesh());
      autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false);
      polyMeshFilter::copySets(newMesh(), mesh);
    }
  }
  if (foamyHexMeshControls().filterFaces())
  {
    labelIOList boundaryPtsIO
    (
      IOobject
      (
        "pointPriority",
        instance,
        time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      labelList(mesh.nPoints(), labelMin)
    );
    forAll(mesh.points(), ptI)
    {
      boundaryPtsIO[ptI] = mesh.pointZones().whichZone(ptI);
    }
    Info<< nl << "Filtering faces on polyMesh" << nl << endl;
    meshFilter.reset(new polyMeshFilter(mesh, boundaryPtsIO));
    meshFilter().filter(nInitialBadFaces);
    {
      const autoPtr<fvMesh>& newMesh = meshFilter().filteredMesh();
      polyTopoChange meshMod(newMesh());
      autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false);
      polyMeshFilter::copySets(newMesh(), mesh);
    }
  }
  timeCheck("After fvMesh filtering");
  mesh.setInstance(instance);
  if (!mesh.write())
  {
    FatalErrorIn("mousse::conformalVoronoiMesh::writeMesh(..)")
      << "Failed writing polyMesh."
      << exit(FatalError);
  }
  else
  {
    Info<< nl << "Written filtered mesh to "
      << mesh.polyMesh::instance() << nl
      << endl;
  }
  {
    pointScalarField boundaryPtsScalarField
    (
      IOobject
      (
        "boundaryPoints_collapsed",
        instance,
        time(),
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      ),
      pointMesh::New(mesh),
      scalar(labelMin)
    );
    labelIOList boundaryPtsIO
    (
      IOobject
      (
        "pointPriority",
        instance,
        time(),
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      ),
      labelList(mesh.nPoints(), labelMin)
    );
    forAll(mesh.points(), ptI)
    {
      boundaryPtsScalarField[ptI] = mesh.pointZones().whichZone(ptI);
      boundaryPtsIO[ptI] = mesh.pointZones().whichZone(ptI);
    }
    boundaryPtsScalarField.write();
    boundaryPtsIO.write();
  }
//    writeCellSizes(mesh);
//    writeCellAlignments(mesh);
//    writeCellCentres(mesh);
  findRemainingProtrusionSet(mesh);
}
void mousse::conformalVoronoiMesh::writeCellSizes
(
  const fvMesh& mesh
) const
{
  {
    timeCheck("Start writeCellSizes");
    Info<< nl << "Create targetCellSize volScalarField" << endl;
    volScalarField targetCellSize
    (
      IOobject
      (
        "targetCellSize",
        mesh.polyMesh::instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      ),
      mesh,
      dimensionedScalar("cellSize", dimLength, 0),
      zeroGradientFvPatchScalarField::typeName
    );
    scalarField& cellSize = targetCellSize.internalField();
    const vectorField& C = mesh.cellCentres();
    forAll(cellSize, i)
    {
      cellSize[i] = cellShapeControls().cellSize(C[i]);
    }
    // Info<< nl << "Create targetCellVolume volScalarField" << endl;
    // volScalarField targetCellVolume
    // (
    //     IOobject
    //     (
    //         "targetCellVolume",
    //         mesh.polyMesh::instance(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh,
    //     dimensionedScalar("cellVolume", dimLength, 0),
    //     zeroGradientFvPatchScalarField::typeName
    // );
    // targetCellVolume.internalField() = pow3(cellSize);
    // Info<< nl << "Create actualCellVolume volScalarField" << endl;
    // volScalarField actualCellVolume
    // (
    //     IOobject
    //     (
    //         "actualCellVolume",
    //         mesh.polyMesh::instance(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh,
    //     dimensionedScalar("cellVolume", dimVolume, 0),
    //     zeroGradientFvPatchScalarField::typeName
    // );
    // actualCellVolume.internalField() = mesh.cellVolumes();
    // Info<< nl << "Create equivalentCellSize volScalarField" << endl;
    // volScalarField equivalentCellSize
    // (
    //     IOobject
    //     (
    //         "equivalentCellSize",
    //         mesh.polyMesh::instance(),
    //         mesh,
    //         IOobject::NO_READ,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh,
    //     dimensionedScalar("cellSize", dimLength, 0),
    //     zeroGradientFvPatchScalarField::typeName
    // );
    // equivalentCellSize.internalField() = pow
    // (
    //     actualCellVolume.internalField(),
    //     1.0/3.0
    // );
    targetCellSize.correctBoundaryConditions();
    // targetCellVolume.correctBoundaryConditions();
    // actualCellVolume.correctBoundaryConditions();
    // equivalentCellSize.correctBoundaryConditions();
    targetCellSize.write();
    // targetCellVolume.write();
    // actualCellVolume.write();
    // equivalentCellSize.write();
  }
  // {
  //     polyMesh tetMesh
  //     (
  //         IOobject
  //         (
  //             "tetDualMesh",
  //             runTime_.constant(),
  //             runTime_,
  //             IOobject::MUST_READ
  //         )
  //     );
  //     pointMesh ptMesh(tetMesh);
  //     pointScalarField ptTargetCellSize
  //     (
  //         IOobject
  //         (
  //             "ptTargetCellSize",
  //             runTime_.timeName(),
  //             tetMesh,
  //             IOobject::NO_READ,
  //             IOobject::AUTO_WRITE
  //         ),
  //         ptMesh,
  //         dimensionedScalar("ptTargetCellSize", dimLength, 0),
  //         pointPatchVectorField::calculatedType()
  //     );
  //     scalarField& cellSize = ptTargetCellSize.internalField();
  //     const vectorField& P = tetMesh.points();
  //     forAll(cellSize, i)
  //     {
  //         cellSize[i] = cellShapeControls().cellSize(P[i]);
  //     }
  //     ptTargetCellSize.write();
  // }
}
void mousse::conformalVoronoiMesh::writeCellAlignments
(
  const fvMesh& mesh
) const
{
//    Info<< nl << "Create cellAlignments volTensorField" << endl;
//
//    volTensorField cellAlignments
//    (
//        IOobject
//        (
//            "cellAlignments",
//            mesh.polyMesh::instance(),
//            mesh,
//            IOobject::NO_READ,
//            IOobject::AUTO_WRITE
//        ),
//        mesh,
//        tensor::I,
//        zeroGradientFvPatchTensorField::typeName
//    );
//
//    tensorField& cellAlignment = cellAlignments.internalField();
//
//    const vectorField& C = mesh.cellCentres();
//
//    vectorField xDir(cellAlignment.size());
//    vectorField yDir(cellAlignment.size());
//    vectorField zDir(cellAlignment.size());
//
//    forAll(cellAlignment, i)
//    {
//        cellAlignment[i] = cellShapeControls().cellAlignment(C[i]);
//        xDir[i] = cellAlignment[i] & vector(1, 0, 0);
//        yDir[i] = cellAlignment[i] & vector(0, 1, 0);
//        zDir[i] = cellAlignment[i] & vector(0, 0, 1);
//    }
//
//    OFstream xStr("xDir.obj");
//    OFstream yStr("yDir.obj");
//    OFstream zStr("zDir.obj");
//
//    forAll(xDir, i)
//    {
//        meshTools::writeOBJ(xStr, C[i], C[i] + xDir[i]);
//        meshTools::writeOBJ(yStr, C[i], C[i] + yDir[i]);
//        meshTools::writeOBJ(zStr, C[i], C[i] + zDir[i]);
//    }
//
//    cellAlignments.correctBoundaryConditions();
//
//    cellAlignments.write();
}
void mousse::conformalVoronoiMesh::writeCellCentres
(
  const fvMesh& mesh
) const
{
  Info<< "Writing components of cellCentre positions to volScalarFields"
    << " ccx, ccy, ccz in " <<  runTime_.timeName() << endl;
  for (direction i=0; i<vector::nComponents; i++)
  {
    volScalarField cci
    (
      IOobject
      (
        "cc" + word(vector::componentNames[i]),
        runTime_.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      ),
      mesh.C().component(i)
    );
    cci.write();
  }
}
mousse::labelHashSet mousse::conformalVoronoiMesh::findRemainingProtrusionSet
(
  const polyMesh& mesh
) const
{
  timeCheck("Start findRemainingProtrusionSet");
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  labelHashSet protrudingBoundaryPoints;
  forAll(patches, patchI)
  {
    const polyPatch& patch = patches[patchI];
    forAll(patch.localPoints(), pLPI)
    {
      label meshPtI = patch.meshPoints()[pLPI];
      const mousse::point& pt = patch.localPoints()[pLPI];
      if
      (
        geometryToConformTo_.wellOutside
        (
          pt,
          sqr(targetCellSize(pt))
        )
      )
      {
        protrudingBoundaryPoints.insert(meshPtI);
      }
    }
  }
  cellSet protrudingCells
  (
    mesh,
    "foamyHexMesh_remainingProtrusions",
    mesh.nCells()/1000
  );
  forAllConstIter(labelHashSet, protrudingBoundaryPoints, iter)
  {
    const label pointI = iter.key();
    const labelList& pCells = mesh.pointCells()[pointI];
    forAll(pCells, pCI)
    {
      protrudingCells.insert(pCells[pCI]);
    }
  }
  label protrudingCellsSize = protrudingCells.size();
  reduce(protrudingCellsSize, sumOp<label>());
  if (foamyHexMeshControls().objOutput() && protrudingCellsSize > 0)
  {
    Info<< nl << "Found " << protrudingCellsSize
      << " cells protruding from the surface, writing cellSet "
      << protrudingCells.name()
      << endl;
    protrudingCells.write();
  }
  return protrudingCells;
}
void mousse::conformalVoronoiMesh::writePointPairs
(
  const fileName& fName
) const
{
  OBJstream os(fName);
  for
  (
    Delaunay::Finite_edges_iterator eit = finite_edges_begin();
    eit != finite_edges_end();
    ++eit
  )
  {
    Cell_handle c = eit->first;
    Vertex_handle vA = c->vertex(eit->second);
    Vertex_handle vB = c->vertex(eit->third);
    if (ptPairs_.isPointPair(vA, vB))
    {
      os.write
      (
        linePointRef(topoint(vA->point()), topoint(vB->point()))
      );
    }
  }
}