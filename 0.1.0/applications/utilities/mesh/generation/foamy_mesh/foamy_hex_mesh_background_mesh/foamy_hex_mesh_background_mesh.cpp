// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "tri_surface.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "cell_shape_control.hpp"
#include "background_mesh_decomposition.hpp"
#include "cell_shape.hpp"
#include "cell_modeller.hpp"
#include "dynamic_field.hpp"
#include "iso_surface_cell.hpp"
#include "vtk_surface_writer.hpp"
#include "sync_tools.hpp"
using namespace mousse;
// Tolerance (as fraction of the bounding box). Needs to be fairly lax since
// usually meshes get written with limited precision (6 digits)
static const scalar defaultMergeTol = 1e-6;
// Get merging distance when matching face centres
scalar getMergeDistance
(
  const argList& args,
  const Time& runTime,
  const boundBox& bb
)
{
  scalar mergeTol = defaultMergeTol;
  args.optionReadIfPresent("mergeTol", mergeTol);
  scalar writeTol =
    mousse::pow(scalar(10.0), -scalar(IOstream::defaultPrecision()));
  Info<< "Merge tolerance : " << mergeTol << nl
    << "Write tolerance : " << writeTol << endl;
  if (runTime.writeFormat() == IOstream::ASCII && mergeTol < writeTol)
  {
    FATAL_ERROR_IN("getMergeDistance")
      << "Your current settings specify ASCII writing with "
      << IOstream::defaultPrecision() << " digits precision." << endl
      << "Your merging tolerance (" << mergeTol << ") is finer than this."
      << endl
      << "Please change your writeFormat to binary"
      << " or increase the writePrecision" << endl
      << "or adjust the merge tolerance (-mergeTol)."
      << exit(FatalError);
  }
  scalar mergeDist = mergeTol * bb.mag();
  Info<< "Overall meshes bounding box : " << bb << nl
    << "Relative tolerance          : " << mergeTol << nl
    << "Absolute matching distance  : " << mergeDist << nl
    << endl;
  return mergeDist;
}
void printMeshData(const polyMesh& mesh)
{
  // Collect all data on master
  globalIndex globalCells{mesh.nCells()};
  labelListList patchNeiProcNo{Pstream::nProcs()};
  labelListList patchSize{Pstream::nProcs()};
  const labelList& pPatches = mesh.globalData().processorPatches();
  patchNeiProcNo[Pstream::myProcNo()].setSize(pPatches.size());
  patchSize[Pstream::myProcNo()].setSize(pPatches.size());
  FOR_ALL(pPatches, i)
  {
    const processorPolyPatch& ppp = refCast<const processorPolyPatch>
    (
      mesh.boundaryMesh()[pPatches[i]]
    );
    patchNeiProcNo[Pstream::myProcNo()][i] = ppp.neighbProcNo();
    patchSize[Pstream::myProcNo()][i] = ppp.size();
  }
  Pstream::gatherList(patchNeiProcNo);
  Pstream::gatherList(patchSize);
  // Print stats
  globalIndex globalBoundaryFaces{mesh.nFaces()-mesh.nInternalFaces()};
  label maxProcCells = 0;
  label totProcFaces = 0;
  label maxProcPatches = 0;
  label totProcPatches = 0;
  label maxProcFaces = 0;
  for (label procI = 0; procI < Pstream::nProcs(); procI++)
  {
    Info<< endl
      << "Processor " << procI << nl
      << "    Number of cells = " << globalCells.localSize(procI)
      << endl;
    label nProcFaces = 0;
    const labelList& nei = patchNeiProcNo[procI];
    FOR_ALL(patchNeiProcNo[procI], i)
    {
      Info<< "    Number of faces shared with processor "
        << patchNeiProcNo[procI][i] << " = " << patchSize[procI][i]
        << endl;
      nProcFaces += patchSize[procI][i];
    }
    Info<< "    Number of processor patches = " << nei.size() << nl
      << "    Number of processor faces = " << nProcFaces << nl
      << "    Number of boundary faces = "
      << globalBoundaryFaces.localSize(procI) << endl;
    maxProcCells = max(maxProcCells, globalCells.localSize(procI));
    totProcFaces += nProcFaces;
    totProcPatches += nei.size();
    maxProcPatches = max(maxProcPatches, nei.size());
    maxProcFaces = max(maxProcFaces, nProcFaces);
  }
  // Stats
  scalar avgProcCells = scalar(globalCells.size())/Pstream::nProcs();
  scalar avgProcPatches = scalar(totProcPatches)/Pstream::nProcs();
  scalar avgProcFaces = scalar(totProcFaces)/Pstream::nProcs();
  // In case of all faces on one processor. Just to avoid division by 0.
  if (totProcPatches == 0)
  {
    avgProcPatches = 1;
  }
  if (totProcFaces == 0)
  {
    avgProcFaces = 1;
  }
  Info<< nl
    << "Number of processor faces = " << totProcFaces/2 << nl
    << "Max number of cells = " << maxProcCells
    << " (" << 100.0*(maxProcCells-avgProcCells)/avgProcCells
    << "% above average " << avgProcCells << ")" << nl
    << "Max number of processor patches = " << maxProcPatches
    << " (" << 100.0*(maxProcPatches-avgProcPatches)/avgProcPatches
    << "% above average " << avgProcPatches << ")" << nl
    << "Max number of faces between processors = " << maxProcFaces
    << " (" << 100.0*(maxProcFaces-avgProcFaces)/avgProcFaces
    << "% above average " << avgProcFaces << ")" << nl
    << endl;
}
// Return cellID
label cellLabel
(
  const Vector<label>& nCells,
  const label i,
  const label j,
  const label k
)
{
  return i*nCells[1]*nCells[2]+j*nCells[2]+k;
}
label vtxLabel
(
  const Vector<label>& nCells,
  const label i,
  const label j,
  const label k
)
{
  Vector<label> nPoints
  {
    nCells[0]+1,
    nCells[1]+1,
    nCells[2]+1
  };
  return i*nPoints[1]*nPoints[2]+j*nPoints[2]+k;
}
autoPtr<polyMesh> generateHexMesh
(
  const IOobject& io,
  const point& origin,
  const vector& cellSize,
  const Vector<label>& nCells
)
{
  pointField points;
  if (nCells[0]+nCells[1]+nCells[2] > 0)
  {
    points.setSize((nCells[0]+1)*(nCells[1]+1)*(nCells[2]+1));
    // Generate points
    for (label i = 0; i <= nCells[0]; i++)
    {
      for (label j = 0; j <= nCells[1]; j++)
      {
        for (label k = 0; k <= nCells[2]; k++)
        {
          point pt = origin;
          pt.x() += i*cellSize[0];
          pt.y() += j*cellSize[1];
          pt.z() += k*cellSize[2];
          points[vtxLabel(nCells, i, j, k)] = pt;
        }
      }
    }
  }
  const cellModel& hex = *(cellModeller::lookup("hex"));
  cellShapeList cellShapes(nCells[0]*nCells[1]*nCells[2]);
  labelList hexPoints(8);
  label cellI = 0;
  for (label i = 0; i < nCells[0]; i++)
  {
    for (label j = 0; j < nCells[1]; j++)
    {
      for (label k = 0; k < nCells[2]; k++)
      {
        hexPoints[0] = vtxLabel(nCells, i,   j,   k);
        hexPoints[1] = vtxLabel(nCells, i+1, j,   k);
        hexPoints[2] = vtxLabel(nCells, i+1, j+1, k);
        hexPoints[3] = vtxLabel(nCells, i,   j+1, k);
        hexPoints[4] = vtxLabel(nCells, i,   j,   k+1);
        hexPoints[5] = vtxLabel(nCells, i+1, j,   k+1);
        hexPoints[6] = vtxLabel(nCells, i+1, j+1, k+1);
        hexPoints[7] = vtxLabel(nCells, i,   j+1, k+1);
        cellShapes[cellI++] = cellShape(hex, hexPoints);
      }
    }
  }
  faceListList boundary(0);
  wordList patchNames(0);
  wordList patchTypes(0);
  word defaultFacesName = "defaultFaces";
  word defaultFacesType = polyPatch::typeName;
  wordList patchPhysicalTypes(0);
  return autoPtr<polyMesh>
  {
    new polyMesh
    {
      io,
      xferMoveTo<pointField>(points),
      cellShapes,
      boundary,
      patchNames,
      patchTypes,
      defaultFacesName,
      defaultFacesType,
      patchPhysicalTypes
    }
  };
}
// Determine for every point a signed distance to the nearest surface
// (outside is positive)
tmp<scalarField> signedDistance
(
  const scalarField& /*distSqr*/,
  const pointField& points,
  const searchableSurfaces& geometry,
  const labelList& surfaces
)
{
  tmp<scalarField> tfld{new scalarField{points.size(), mousse::sqr(GREAT)}};
  scalarField& fld = tfld();
  // Find nearest
  List<pointIndexHit> nearest;
  labelList nearestSurfaces;
  searchableSurfacesQueries::findNearest
  (
    geometry,
    surfaces,
    points,
    scalarField(points.size(), mousse::sqr(GREAT)),//distSqr
    nearestSurfaces,
    nearest
  );
  // Determine sign of nearest. Sort by surface to do this.
  DynamicField<point> surfPoints{points.size()};
  DynamicList<label> surfIndices{points.size()};
  FOR_ALL(surfaces, surfI)
  {
    // Extract points on this surface
    surfPoints.clear();
    surfIndices.clear();
    FOR_ALL(nearestSurfaces, i)
    {
      if (nearestSurfaces[i] == surfI)
      {
        surfPoints.append(points[i]);
        surfIndices.append(i);
      }
    }
    // Calculate sideness of these surface points
    label geomI = surfaces[surfI];
    List<volumeType> volType;
    geometry[geomI].getVolumeType(surfPoints, volType);
    // Push back to original
    FOR_ALL(volType, i)
    {
      label pointI = surfIndices[i];
      scalar dist = mag(points[pointI] - nearest[pointI].hitPoint());
      volumeType vT = volType[i];
      if (vT == volumeType::OUTSIDE)
      {
        fld[pointI] = dist;
      }
      else if (vT == volumeType::INSIDE)
      {
        fld[i] = -dist;
      }
      else
      {
        FATAL_ERROR_IN("signedDistance()")
          << "getVolumeType failure, neither INSIDE or OUTSIDE"
          << exit(FatalError);
      }
    }
  }
  return tfld;
}
// Main program:
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Generate foamyHexMesh-consistent representation of surfaces"
  );
  argList::addBoolOption
  (
    "writeMesh",
    "write the resulting mesh and distance fields"
  );
  argList::addOption
  (
    "mergeTol",
    "scalar",
    "specify the merge distance relative to the bounding box size "
    "(default 1e-6)"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  const bool writeMesh = args.optionFound("writeMesh");
  if (writeMesh)
  {
    Info<< "Writing resulting mesh and cellDistance, pointDistance fields."
      << nl << endl;
  }
  IOdictionary foamyHexMeshDict
  {
    {
      "foamyHexMeshDict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  // Define/load all geometry
  searchableSurfaces allGeometry
  {
    {
      "cvSearchableSurfaces",
      runTime.constant(),
      "triSurface",
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    foamyHexMeshDict.subDict("geometry"),
    foamyHexMeshDict.lookupOrDefault("singleRegionName", true)
  };
  Random rndGen{64293*Pstream::myProcNo()};
  conformationSurfaces geometryToConformTo
  (
    runTime,
    // rndGen,
    allGeometry,
    foamyHexMeshDict.subDict("surfaceConformation")
  );
  cellShapeControl cellShapeControls
  {
    runTime,
    foamyHexMeshDict.subDict("motionControl"),
    allGeometry,
    geometryToConformTo
  };

  // Generate starting block mesh
  vector cellSize;

  {
    const treeBoundBox& bb = geometryToConformTo.globalBounds();
    // Determine the number of cells in each direction.
    const vector span = bb.span();
    vector nScalarCells = span/cellShapeControls.defaultCellSize();
    // Calculate initial cell size to be a little bit smaller than the
    // defaultCellSize to avoid initial refinement triggering.
    Vector<label> nCells = Vector<label>
    (
      label(nScalarCells.x())+2,
      label(nScalarCells.y())+2,
      label(nScalarCells.z())+2
    );
    cellSize = vector
    (
      span[0]/nCells[0],
      span[1]/nCells[1],
      span[2]/nCells[2]
    );
    Info<< "Generating initial hex mesh with" << nl
      << "    bounding box : " << bb << nl
      << "    nCells       : " << nCells << nl
      << "    cellSize     : " << cellSize << nl
      << endl;
    autoPtr<polyMesh> meshPtr
    {
      generateHexMesh
      (
        IOobject
        {
          polyMesh::defaultRegion,
          runTime.constant(),
          runTime
        },
        bb.min(),
        cellSize,
        (
          Pstream::master() ? nCells : Vector<label>(0, 0, 0)
        )
      )
    };
    Info<< "Writing initial hex mesh to " << meshPtr().instance() << nl
      << endl;
    meshPtr().write();
  }
  // Distribute the initial mesh
  if (Pstream::parRun())
  {
    #include "create_mesh.inc"
    Info<< "Loaded mesh:" << endl;
    printMeshData(mesh);
    // Allocate a decomposer
    IOdictionary decompositionDict
    {
      {
        "decomposeParDict",
        runTime.system(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    autoPtr<decompositionMethod> decomposer
    {
      decompositionMethod::New
      (
        decompositionDict
      )
    };
    labelList decomp = decomposer().decompose(mesh, mesh.cellCentres());
    // Global matching tolerance
    const scalar tolDim = getMergeDistance
    (
      args,
      runTime,
      mesh.bounds()
    );
    // Mesh distribution engine
    fvMeshDistribute distributor{mesh, tolDim};
    Info << "Wanted distribution:" << distributor.countCells(decomp) << nl
      << endl;
    // Do actual sending/receiving of mesh
    autoPtr<mapDistributePolyMesh> map = distributor.distribute(decomp);
    // Print some statistics
    //Info<< "After distribution:" << endl;
    //printMeshData(mesh);
    mesh.setInstance(runTime.constant());
    Info<< "Writing redistributed mesh" << nl << endl;
    mesh.write();
  }
  Info<< "Refining backgroud mesh according to cell size specification" << nl
    << endl;
  backgroundMeshDecomposition backgroundMesh
  {
    runTime,
    rndGen,
    geometryToConformTo,
    foamyHexMeshDict
  };
  if (writeMesh)
  {
    runTime++;
    Info<< "Writing mesh to " << runTime.timeName() << endl;
    backgroundMesh.mesh().write();
  }
  const scalar tolDim = getMergeDistance
  (
    args,
    runTime,
    backgroundMesh.mesh().bounds()
  );
  faceList isoFaces;
  pointField isoPoints;

  {
    // Apply a distanceSurface to it.
    const fvMesh& fvm = backgroundMesh.mesh();
    volScalarField cellDistance
    {
      {
        "cellDistance",
        fvm.time().timeName(),
        fvm.time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      fvm,
      dimensionedScalar("zero", dimLength, 0)
    };
    const searchableSurfaces& geometry = geometryToConformTo.geometry();
    const labelList& surfaces = geometryToConformTo.surfaces();
    // Get maximum search size per cell
    scalarField distSqr(cellDistance.size());
    const labelList& cellLevel = backgroundMesh.cellLevel();
    FOR_ALL(cellLevel, cellI)
    {
      // The largest edge of the cell will always be less than the
      // span of the bounding box of the cell.
      distSqr[cellI] = magSqr(cellSize)/std::pow(2, cellLevel[cellI]);
    }

    {
      // Internal field
      cellDistance.internalField() = signedDistance
      (
        distSqr,
        fvm.C(),
        geometry,
        surfaces
      );
      // Patch fields
      FOR_ALL(fvm.C().boundaryField(), patchI)
      {
        const pointField& cc = fvm.C().boundaryField()[patchI];
        fvPatchScalarField& fld = cellDistance.boundaryField()[patchI];
        scalarField patchDistSqr
        (
          fld.patch().patchInternalField(distSqr)
        );
        fld = signedDistance(patchDistSqr, cc, geometry, surfaces);
      }
      // On processor patches the fvm.C() will already be the cell centre
      // on the opposite side so no need to swap cellDistance.
      if (writeMesh)
      {
        cellDistance.write();
      }
    }

    // Distance to points
    pointScalarField pointDistance
    {
      {
        "pointDistance",
        fvm.time().timeName(),
        fvm.time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pointMesh::New(fvm),
      dimensionedScalar("zero", dimLength, 0)
    };

    {
      scalarField pointDistSqr{fvm.nPoints(), -sqr(GREAT)};
      for (label faceI = 0; faceI < fvm.nInternalFaces(); faceI++)
      {
        label own = fvm.faceOwner()[faceI];
        label ownDistSqr = distSqr[own];
        const face& f = fvm.faces()[faceI];
        FOR_ALL(f, fp)
        {
          pointDistSqr[f[fp]] = max(pointDistSqr[f[fp]], ownDistSqr);
        }
      }
      syncTools::syncPointList
      (
        fvm,
        pointDistSqr,
        maxEqOp<scalar>(),
        -sqr(GREAT)             // null value
      );
      pointDistance.internalField() = signedDistance
      (
        pointDistSqr,
        fvm.points(),
        geometry,
        surfaces
      );
      if (writeMesh)
      {
        pointDistance.write();
      }
    }
    isoSurfaceCell iso
    {
      fvm,
      cellDistance,
      pointDistance,
      0,      //distance,
      false   //regularise
    };
    isoFaces.setSize(iso.size());
    FOR_ALL(isoFaces, i)
    {
      isoFaces[i] = iso[i].triFaceFace();
    }
    isoPoints = iso.points();
  }
  pointField mergedPoints;
  faceList mergedFaces;
  labelList pointMergeMap;
  PatchTools::gatherAndMerge
  (
    tolDim,
    primitivePatch
    {
      SubList<face>(isoFaces, isoFaces.size()),
      isoPoints
    },
    mergedPoints,
    mergedFaces,
    pointMergeMap
  );
  vtkSurfaceWriter writer;
  writer.write
  (
    runTime.path(),
    "iso",
    mergedPoints,
    mergedFaces
  );
  Info<< "End\n" << endl;
  return 0;
}
