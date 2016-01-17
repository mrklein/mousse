// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "domain_decomposition.hpp"
#include "dictionary.hpp"
#include "label_io_list.hpp"
#include "processor_poly_patch.hpp"
#include "processor_cyclic_poly_patch.hpp"
#include "fv_mesh.hpp"
#include "os_specific.hpp"
#include "map.hpp"
#include "global_mesh_data.hpp"
#include "dynamic_list.hpp"
#include "fv_field_decomposer.hpp"
#include "ioobject_list.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"
#include "uniform_dimensioned_fields.hpp"
// Private Member Functions 
void mousse::domainDecomposition::mark
(
  const labelList& zoneElems,
  const label zoneI,
  labelList& elementToZone
)
{
  FOR_ALL(zoneElems, i)
  {
    label pointi = zoneElems[i];
    if (elementToZone[pointi] == -1)
    {
      // First occurrence
      elementToZone[pointi] = zoneI;
    }
    else if (elementToZone[pointi] >= 0)
    {
      // Multiple zones
      elementToZone[pointi] = -2;
    }
  }
}
// Constructors 
mousse::domainDecomposition::domainDecomposition(const IOobject& io)
:
  fvMesh{io},
  facesInstancePointsPtr_
  {
    pointsInstance() != facesInstance()
   ? new pointIOField
     {
       // IOobject
       {
         "points",
         facesInstance(),
         polyMesh::meshSubDir,
         *this,
         IOobject::MUST_READ,
         IOobject::NO_WRITE,
         false
       }
     }
    : NULL
  },
  decompositionDict_
  {
    // IOobject
    {
      "decomposeParDict",
      time().system(),
      *this,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  },
  nProcs_{readInt(decompositionDict_.lookup("numberOfSubdomains"))},
  distributed_{false},
  cellToProc_{nCells()},
  procPointAddressing_{nProcs_},
  procFaceAddressing_{nProcs_},
  procCellAddressing_{nProcs_},
  procPatchSize_{nProcs_},
  procPatchStartIndex_{nProcs_},
  procNeighbourProcessors_{nProcs_},
  procProcessorPatchSize_{nProcs_},
  procProcessorPatchStartIndex_{nProcs_},
  procProcessorPatchSubPatchIDs_{nProcs_},
  procProcessorPatchSubPatchStarts_{nProcs_}
{
  decompositionDict_.readIfPresent("distributed", distributed_);
}
// Destructor 
mousse::domainDecomposition::~domainDecomposition()
{}
// Member Functions 
bool mousse::domainDecomposition::writeDecomposition(const bool decomposeSets)
{
  Info<< "\nConstructing processor meshes" << endl;
  // Mark point/faces/cells that are in zones.
  // -1   : not in zone
  // -2   : in multiple zones
  // >= 0 : in single given zone
  // This will give direct lookup of elements that are in a single zone
  // and we'll only have to revert back to searching through all zones
  // for the duplicate elements
  // Point zones
  labelList pointToZone{points().size(), -1};
  FOR_ALL(pointZones(), zoneI)
  {
    mark(pointZones()[zoneI], zoneI, pointToZone);
  }
  // Face zones
  labelList faceToZone{faces().size(), -1};
  FOR_ALL(faceZones(), zoneI)
  {
    mark(faceZones()[zoneI], zoneI, faceToZone);
  }
  // Cell zones
  labelList cellToZone{nCells(), -1};
  FOR_ALL(cellZones(), zoneI)
  {
    mark(cellZones()[zoneI], zoneI, cellToZone);
  }
  PtrList<const cellSet> cellSets;
  PtrList<const faceSet> faceSets;
  PtrList<const pointSet> pointSets;
  if (decomposeSets)
  {
    // Read sets
    IOobjectList objects{*this, facesInstance(), "polyMesh/sets"};

    {
      IOobjectList cSets{objects.lookupClass(cellSet::typeName)};
      FOR_ALL_CONST_ITER(IOobjectList, cSets, iter)
      {
        cellSets.append(new cellSet{*iter()});
      }
    }

    {
      IOobjectList fSets{objects.lookupClass(faceSet::typeName)};
      FOR_ALL_CONST_ITER(IOobjectList, fSets, iter)
      {
        faceSets.append(new faceSet{*iter()});
      }
    }

    {
      IOobjectList pSets{objects.lookupClass(pointSet::typeName)};
      FOR_ALL_CONST_ITER(IOobjectList, pSets, iter)
      {
        pointSets.append(new pointSet{*iter()});
      }
    }
  }

  autoPtr<labelIOList> cellLevelPtr;

  {
    IOobject io
    {
      "cellLevel",
      facesInstance(),
      polyMesh::meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    };
    if (io.headerOk())
    {
      Info<< "Reading hexRef8 data : " << io.name() << endl;
      cellLevelPtr.reset(new labelIOList{io});
    }
  }
  autoPtr<labelIOList> pointLevelPtr;

  {
    IOobject io
    {
      "pointLevel",
      facesInstance(),
      polyMesh::meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    };
    if (io.headerOk())
    {
      Info<< "Reading hexRef8 data : " << io.name() << endl;
      pointLevelPtr.reset(new labelIOList{io});
    }
  }

  autoPtr<uniformDimensionedScalarField> level0EdgePtr;

  {
    IOobject io
    {
      "level0Edge",
      facesInstance(),
      polyMesh::meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    };
    if (io.headerOk())
    {
      Info<< "Reading hexRef8 data : " << io.name() << endl;
      level0EdgePtr.reset(new uniformDimensionedScalarField{io});
    }
  }
  label maxProcCells = 0;
  label totProcFaces = 0;
  label maxProcPatches = 0;
  label totProcPatches = 0;
  label maxProcFaces = 0;
  // Write out the meshes
  for (label procI = 0; procI < nProcs_; procI++)
  {
    // Create processor points
    const labelList& curPointLabels = procPointAddressing_[procI];
    const pointField& meshPoints = points();
    labelList pointLookup{nPoints(), -1};
    pointField procPoints{curPointLabels.size()};
    FOR_ALL(curPointLabels, pointi)
    {
      procPoints[pointi] = meshPoints[curPointLabels[pointi]];
      pointLookup[curPointLabels[pointi]] = pointi;
    }
    // Create processor faces
    const labelList& curFaceLabels = procFaceAddressing_[procI];
    const faceList& meshFaces = faces();
    labelList faceLookup{nFaces(), -1};
    faceList procFaces{curFaceLabels.size()};
    FOR_ALL(curFaceLabels, facei)
    {
      // Mark the original face as used
      // Remember to decrement the index by one (turning index)
      //
      label curF = mag(curFaceLabels[facei]) - 1;
      faceLookup[curF] = facei;
      // get the original face
      labelList origFaceLabels;
      if (curFaceLabels[facei] >= 0)
      {
        // face not turned
        origFaceLabels = meshFaces[curF];
      }
      else
      {
        origFaceLabels = meshFaces[curF].reverseFace();
      }
      // translate face labels into local point list
      face& procFaceLabels = procFaces[facei];
      procFaceLabels.setSize(origFaceLabels.size());
      FOR_ALL(origFaceLabels, pointi)
      {
        procFaceLabels[pointi] = pointLookup[origFaceLabels[pointi]];
      }
    }
    // Create processor cells
    const labelList& curCellLabels = procCellAddressing_[procI];
    const cellList& meshCells = cells();
    cellList procCells{curCellLabels.size()};
    FOR_ALL(curCellLabels, celli)
    {
      const labelList& origCellLabels = meshCells[curCellLabels[celli]];
      cell& curCell = procCells[celli];
      curCell.setSize(origCellLabels.size());
      FOR_ALL(origCellLabels, cellFaceI)
      {
        curCell[cellFaceI] = faceLookup[origCellLabels[cellFaceI]];
      }
    }
    // Create processor mesh without a boundary
    fileName processorCasePath
    (
      time().caseName()/fileName(word("processor") + mousse::name(procI))
    );
    // make the processor directory
    mkDir(time().rootPath()/processorCasePath);
    // create a database
    Time processorDb
    {
      Time::controlDictName,
      time().rootPath(),
      processorCasePath,
      word("system"),
      word("constant")
    };
    processorDb.setTime(time());
    // create the mesh. Two situations:
    // - points and faces come from the same time ('instance'). The mesh
    //   will get constructed in the same instance.
    // - points come from a different time (moving mesh cases).
    //   It will read the points belonging to the faces instance and
    //   construct the procMesh with it which then gets handled as above.
    //   (so with 'old' geometry).
    //   Only at writing time will it additionally write the current
    //   points.
    autoPtr<polyMesh> procMeshPtr;
    if (facesInstancePointsPtr_.valid())
    {
      // Construct mesh from facesInstance.
      pointField facesInstancePoints
      (
        facesInstancePointsPtr_(),
        curPointLabels
      );
      procMeshPtr.reset
      (
        new polyMesh
        {
          // IOobject
          {
            this->polyMesh::name(), // region of undecomposed mesh
            facesInstance(),
            processorDb
          },
          xferMove(facesInstancePoints),
          xferMove(procFaces),
          xferMove(procCells)
        }
      );
    }
    else
    {
      procMeshPtr.reset
      (
        new polyMesh
        {
          // IOobject
          {
            this->polyMesh::name(), // region of undecomposed mesh
            facesInstance(),
            processorDb
          },
          xferMove(procPoints),
          xferMove(procFaces),
          xferMove(procCells)
        }
      );
    }
    polyMesh& procMesh = procMeshPtr();
    // Create processor boundary patches
    const labelList& curPatchSizes = procPatchSize_[procI];
    const labelList& curPatchStarts = procPatchStartIndex_[procI];
    const labelList& curNeighbourProcessors =
      procNeighbourProcessors_[procI];
    const labelList& curProcessorPatchSizes =
      procProcessorPatchSize_[procI];
    const labelList& curProcessorPatchStarts =
      procProcessorPatchStartIndex_[procI];
    const labelListList& curSubPatchIDs =
      procProcessorPatchSubPatchIDs_[procI];
    const labelListList& curSubStarts =
      procProcessorPatchSubPatchStarts_[procI];
    const polyPatchList& meshPatches = boundaryMesh();
    // Count the number of inter-proc patches
    label nInterProcPatches = 0;
    FOR_ALL(curSubPatchIDs, procPatchI)
    {
      nInterProcPatches += curSubPatchIDs[procPatchI].size();
    }
    List<polyPatch*> procPatches
    {
      curPatchSizes.size()
        + nInterProcPatches,          //curProcessorPatchSizes.size(),
      reinterpret_cast<polyPatch*>(0)
    };
    label nPatches = 0;
    FOR_ALL(curPatchSizes, patchi)
    {
      // Get the face labels consistent with the field mapping
      // (reuse the patch field mappers)
      const polyPatch& meshPatch = meshPatches[patchi];
      fvFieldDecomposer::patchFieldDecomposer patchMapper
      (
        SubList<label>
        {
          curFaceLabels,
          curPatchSizes[patchi],
          curPatchStarts[patchi]
        },
        meshPatch.start()
      );
      // Map existing patches
      procPatches[nPatches] = meshPatch.clone
      (
        procMesh.boundaryMesh(),
        nPatches,
        patchMapper.directAddressing(),
        curPatchStarts[patchi]
      ).ptr();
      nPatches++;
    }
    FOR_ALL(curProcessorPatchSizes, procPatchI)
    {
      const labelList& subPatchID = curSubPatchIDs[procPatchI];
      const labelList& subStarts = curSubStarts[procPatchI];
      label curStart = curProcessorPatchStarts[procPatchI];
      FOR_ALL(subPatchID, i)
      {
        label size =
        (
          i < subPatchID.size()-1
         ? subStarts[i+1] - subStarts[i]
         : curProcessorPatchSizes[procPatchI] - subStarts[i]
        );
        if (subPatchID[i] == -1)
        {
          // From internal faces
          procPatches[nPatches] =
            new processorPolyPatch
            {
              word("procBoundary") + mousse::name(procI)
             + "to"
             + mousse::name(curNeighbourProcessors[procPatchI]),
              size,
              curStart,
              nPatches,
              procMesh.boundaryMesh(),
              static_cast<int>(procI),
              static_cast<int>(curNeighbourProcessors[procPatchI])
            };
        }
        else
        {
          const coupledPolyPatch& pcPatch
            = refCast<const coupledPolyPatch>
             (
               boundaryMesh()[subPatchID[i]]
             );
          // From cyclic
          const word& referPatch = pcPatch.name();
          procPatches[nPatches] =
            new processorCyclicPolyPatch
            {
              word("procBoundary") + mousse::name(procI)
             + "to"
             + mousse::name(curNeighbourProcessors[procPatchI])
             + "through"
             + referPatch,
              size,
              curStart,
              nPatches,
              procMesh.boundaryMesh(),
              static_cast<int>(procI),
              static_cast<int>(curNeighbourProcessors[procPatchI]),
              referPatch,
              pcPatch.transform()
            };
        }
        curStart += size;
        nPatches++;
      }
    }
    // Add boundary patches
    procMesh.addPatches(procPatches);
    // Create and add zones
    // Point zones
    {
      const pointZoneMesh& pz = pointZones();
      // Go through all the zoned points and find out if they
      // belong to a zone.  If so, add it to the zone as
      // necessary
      List<DynamicList<label>> zonePoints{pz.size()};
      // Estimate size
      FOR_ALL(zonePoints, zoneI)
      {
        zonePoints[zoneI].setCapacity(pz[zoneI].size() / nProcs_);
      }
      // Use the pointToZone map to find out the single zone (if any),
      // use slow search only for shared points.
      FOR_ALL(curPointLabels, pointi)
      {
        label curPoint = curPointLabels[pointi];
        label zoneI = pointToZone[curPoint];
        if (zoneI >= 0)
        {
          // Single zone.
          zonePoints[zoneI].append(pointi);
        }
        else if (zoneI == -2)
        {
          // Multiple zones. Lookup.
          FOR_ALL(pz, zoneI)
          {
            label index = pz[zoneI].whichPoint(curPoint);
            if (index != -1)
            {
              zonePoints[zoneI].append(pointi);
            }
          }
        }
      }
      procMesh.pointZones().clearAddressing();
      procMesh.pointZones().setSize(zonePoints.size());
      FOR_ALL(zonePoints, zoneI)
      {
        procMesh.pointZones().set
        (
          zoneI,
          pz[zoneI].clone
          (
            procMesh.pointZones(),
            zoneI,
            zonePoints[zoneI].shrink()
          )
        );
      }
      if (pz.size())
      {
        // Force writing on all processors
        procMesh.pointZones().writeOpt() = IOobject::AUTO_WRITE;
      }
    }
    // Face zones
    {
      const faceZoneMesh& fz = faceZones();
      // Go through all the zoned face and find out if they
      // belong to a zone.  If so, add it to the zone as
      // necessary
      List<DynamicList<label> > zoneFaces{fz.size()};
      List<DynamicList<bool> > zoneFaceFlips{fz.size()};
      // Estimate size
      FOR_ALL(zoneFaces, zoneI)
      {
        label procSize = fz[zoneI].size() / nProcs_;
        zoneFaces[zoneI].setCapacity(procSize);
        zoneFaceFlips[zoneI].setCapacity(procSize);
      }
      // Go through all the zoned faces and find out if they
      // belong to a zone.  If so, add it to the zone as
      // necessary
      FOR_ALL(curFaceLabels, facei)
      {
        // Remember to decrement the index by one (turning index)
        //
        label curF = mag(curFaceLabels[facei]) - 1;
        label zoneI = faceToZone[curF];
        if (zoneI >= 0)
        {
          // Single zone. Add the face
          zoneFaces[zoneI].append(facei);
          label index = fz[zoneI].whichFace(curF);
          bool flip = fz[zoneI].flipMap()[index];
          if (curFaceLabels[facei] < 0)
          {
            flip = !flip;
          }
          zoneFaceFlips[zoneI].append(flip);
        }
        else if (zoneI == -2)
        {
          // Multiple zones. Lookup.
          FOR_ALL(fz, zoneI)
          {
            label index = fz[zoneI].whichFace(curF);
            if (index != -1)
            {
              zoneFaces[zoneI].append(facei);
              bool flip = fz[zoneI].flipMap()[index];
              if (curFaceLabels[facei] < 0)
              {
                flip = !flip;
              }
              zoneFaceFlips[zoneI].append(flip);
            }
          }
        }
      }
      procMesh.faceZones().clearAddressing();
      procMesh.faceZones().setSize(zoneFaces.size());
      FOR_ALL(zoneFaces, zoneI)
      {
        procMesh.faceZones().set
        (
          zoneI,
          fz[zoneI].clone
          (
            zoneFaces[zoneI].shrink(),          // addressing
            zoneFaceFlips[zoneI].shrink(),      // flipmap
            zoneI,
            procMesh.faceZones()
          )
        );
      }
      if (fz.size())
      {
        // Force writing on all processors
        procMesh.faceZones().writeOpt() = IOobject::AUTO_WRITE;
      }
    }
    // Cell zones
    {
      const cellZoneMesh& cz = cellZones();
      // Go through all the zoned cells and find out if they
      // belong to a zone.  If so, add it to the zone as
      // necessary
      List<DynamicList<label> > zoneCells{cz.size()};
      // Estimate size
      FOR_ALL(zoneCells, zoneI)
      {
        zoneCells[zoneI].setCapacity(cz[zoneI].size() / nProcs_);
      }
      FOR_ALL(curCellLabels, celli)
      {
        label curCellI = curCellLabels[celli];
        label zoneI = cellToZone[curCellI];
        if (zoneI >= 0)
        {
          // Single zone.
          zoneCells[zoneI].append(celli);
        }
        else if (zoneI == -2)
        {
          // Multiple zones. Lookup.
          FOR_ALL(cz, zoneI)
          {
            label index = cz[zoneI].whichCell(curCellI);
            if (index != -1)
            {
              zoneCells[zoneI].append(celli);
            }
          }
        }
      }
      procMesh.cellZones().clearAddressing();
      procMesh.cellZones().setSize(zoneCells.size());
      FOR_ALL(zoneCells, zoneI)
      {
        procMesh.cellZones().set
        (
          zoneI,
          cz[zoneI].clone
          (
            zoneCells[zoneI].shrink(),
            zoneI,
            procMesh.cellZones()
          )
        );
      }
      if (cz.size())
      {
        // Force writing on all processors
        procMesh.cellZones().writeOpt() = IOobject::AUTO_WRITE;
      }
    }
    // Set the precision of the points data to 10
    IOstream::defaultPrecision(10);
    procMesh.write();
    // Write points if pointsInstance differing from facesInstance
    if (facesInstancePointsPtr_.valid())
    {
      pointIOField pointsInstancePoints
      {
        // IOobject
        {
          "points",
          pointsInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        xferMove(procPoints)
      };
      pointsInstancePoints.write();
    }
    // Decompose any sets
    if (decomposeSets)
    {
      FOR_ALL(cellSets, i)
      {
        const cellSet& cs = cellSets[i];
        cellSet set{procMesh, cs.name(), cs.size()/nProcs_};
        FOR_ALL(curCellLabels, i)
        {
          if (cs.found(curCellLabels[i]))
          {
            set.insert(i);
          }
        }
        set.write();
      }
      FOR_ALL(faceSets, i)
      {
        const faceSet& cs = faceSets[i];
        faceSet set{procMesh, cs.name(), cs.size()/nProcs_};
        FOR_ALL(curFaceLabels, i)
        {
          if (cs.found(mag(curFaceLabels[i])-1))
          {
            set.insert(i);
          }
        }
        set.write();
      }
      FOR_ALL(pointSets, i)
      {
        const pointSet& cs = pointSets[i];
        pointSet set(procMesh, cs.name(), cs.size()/nProcs_);
        FOR_ALL(curPointLabels, i)
        {
          if (cs.found(curPointLabels[i]))
          {
            set.insert(i);
          }
        }
        set.write();
      }
    }
    // hexRef8 data
    if (cellLevelPtr.valid())
    {
      labelIOList
      (
        IOobject
        {
          cellLevelPtr().name(),
          facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        UIndirectList<label>
        {
          cellLevelPtr(),
          procCellAddressing_[procI]
        }()
      ).write();
    }
    if (pointLevelPtr.valid())
    {
      labelIOList
      {
        // IOobject
        {
          pointLevelPtr().name(),
          facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        UIndirectList<label>
        {
          pointLevelPtr(),
          procPointAddressing_[procI]
        }()
      }.write();
    }
    if (level0EdgePtr.valid())
    {
      uniformDimensionedScalarField
      (
        // IOobject
        {
          level0EdgePtr().name(),
          facesInstance(),
          polyMesh::meshSubDir,
          procMesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        level0EdgePtr()
      ).write();
    }
    // Statistics
    Info<< endl
      << "Processor " << procI << nl
      << "    Number of cells = " << procMesh.nCells()
      << endl;
    maxProcCells = max(maxProcCells, procMesh.nCells());
    label nBoundaryFaces = 0;
    label nProcPatches = 0;
    label nProcFaces = 0;
    FOR_ALL(procMesh.boundaryMesh(), patchi)
    {
      if (isA<processorPolyPatch>(procMesh.boundaryMesh()[patchi]))
      {
        const processorPolyPatch& ppp =
        refCast<const processorPolyPatch>
        (
          procMesh.boundaryMesh()[patchi]
        );
        Info<< "    Number of faces shared with processor "
          << ppp.neighbProcNo() << " = " << ppp.size() << endl;
        nProcPatches++;
        nProcFaces += ppp.size();
      }
      else
      {
        nBoundaryFaces += procMesh.boundaryMesh()[patchi].size();
      }
    }
    Info<< "    Number of processor patches = " << nProcPatches << nl
      << "    Number of processor faces = " << nProcFaces << nl
      << "    Number of boundary faces = " << nBoundaryFaces << endl;
    totProcFaces += nProcFaces;
    totProcPatches += nProcPatches;
    maxProcPatches = max(maxProcPatches, nProcPatches);
    maxProcFaces = max(maxProcFaces, nProcFaces);
    // create and write the addressing information
    labelIOList pointProcAddressing
    {
      // IOobject
      {
        "pointProcAddressing",
        procMesh.facesInstance(),
        procMesh.meshSubDir,
        procMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procPointAddressing_[procI]
    };
    pointProcAddressing.write();
    labelIOList faceProcAddressing
    {
      // IOobject
      {
        "faceProcAddressing",
        procMesh.facesInstance(),
        procMesh.meshSubDir,
        procMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procFaceAddressing_[procI]
    };
    faceProcAddressing.write();
    labelIOList cellProcAddressing
    {
      // IOobject
      {
        "cellProcAddressing",
        procMesh.facesInstance(),
        procMesh.meshSubDir,
        procMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procCellAddressing_[procI]
    };
    cellProcAddressing.write();
    // Write patch map for backwards compatibility.
    // (= identity map for original patches, -1 for processor patches)
    label nMeshPatches = curPatchSizes.size();
    labelList procBoundaryAddressing{identity(nMeshPatches)};
    procBoundaryAddressing.setSize(nMeshPatches+nProcPatches, -1);
    labelIOList boundaryProcAddressing
    {
      // IOobject
      {
        "boundaryProcAddressing",
        procMesh.facesInstance(),
        procMesh.meshSubDir,
        procMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procBoundaryAddressing
    };
    boundaryProcAddressing.write();
  }
  scalar avgProcCells = scalar(nCells())/nProcs_;
  scalar avgProcPatches = scalar(totProcPatches)/nProcs_;
  scalar avgProcFaces = scalar(totProcFaces)/nProcs_;
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
  return true;
}
