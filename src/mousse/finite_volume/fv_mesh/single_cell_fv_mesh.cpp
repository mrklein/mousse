// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_cell_fv_mesh.hpp"
#include "sync_tools.hpp"
#include "uindirect_primitive_patch.hpp"
#include "vector_field.hpp"
#include "point_field.hpp"


// Private Member Functions 
// Conversion is a two step process:
// - from original (fine) patch faces to agglomerations (aggloms might not
//   be in correct patch order)
// - from agglomerations to coarse patch faces
void mousse::singleCellFvMesh::agglomerateMesh
(
  const fvMesh& mesh,
  const labelListList& agglom
)
{
  const polyBoundaryMesh& oldPatches = mesh.boundaryMesh();
  // Check agglomeration within patch face range and continuous
  labelList nAgglom{oldPatches.size(), 0};
  FOR_ALL(oldPatches, patchI) {
    const polyPatch& pp = oldPatches[patchI];
    if (pp.size() > 0) {
      nAgglom[patchI] = max(agglom[patchI])+1;
      FOR_ALL(pp, i) {
        if (agglom[patchI][i] < 0  || agglom[patchI][i] >= pp.size()) {
          FATAL_ERROR_IN
          (
            "singleCellFvMesh::agglomerateMesh(..)"
          )
          << "agglomeration on patch " << patchI
          << " is out of range 0.." << pp.size()-1
          << exit(FatalError);
        }
      }
    }
  }
  // Check agglomeration is sync
  {
    // Get neighbouring agglomeration
    labelList nbrAgglom{mesh.nFaces()-mesh.nInternalFaces()};
    FOR_ALL(oldPatches, patchI) {
      const polyPatch& pp = oldPatches[patchI];
      if (pp.coupled()) {
        label offset = pp.start() - mesh.nInternalFaces();
        FOR_ALL(pp, i) {
          nbrAgglom[offset+i] = agglom[patchI][i];
        }
      }
    }
    syncTools::swapBoundaryFaceList(mesh, nbrAgglom);
    // Get correspondence between this agglomeration and remote one
    Map<label> localToNbr{nbrAgglom.size()/10};
    FOR_ALL(oldPatches, patchI) {
      const polyPatch& pp = oldPatches[patchI];
      if (pp.coupled()) {
        label offset = pp.start() - mesh.nInternalFaces();
        FOR_ALL(pp, i) {
          label bFaceI = offset+i;
          label myZone = agglom[patchI][i];
          label nbrZone = nbrAgglom[bFaceI];
          Map<label>::const_iterator iter = localToNbr.find(myZone);
          if (iter == localToNbr.end()) {
            // First occurence of this zone. Store correspondence
            // to remote zone number.
            localToNbr.insert(myZone, nbrZone);
          } else {
            // Check that zone numbers are still the same.
            if (iter() != nbrZone) {
              FATAL_ERROR_IN
              (
                "singleCellFvMesh::agglomerateMesh(..)"
              )
              << "agglomeration is not synchronised across"
              << " coupled patch " << pp.name()
              << endl
              << "Local agglomeration " << myZone
              << ". Remote agglomeration " << nbrZone
              << exit(FatalError);
            }
          }
        }
      }
    }
  }
  label coarseI = 0;
  FOR_ALL(nAgglom, patchI) {
    coarseI += nAgglom[patchI];
  }
  // New faces
  faceList patchFaces{coarseI};
  // New patch start and size
  labelList patchStarts{oldPatches.size()};
  labelList patchSizes{oldPatches.size()};
  // From new patch face back to agglomeration
  patchFaceMap_.setSize(oldPatches.size());
  // From fine face to coarse face (or -1)
  reverseFaceMap_.setSize(mesh.nFaces());
  reverseFaceMap_.labelList::operator=(-1);
  // Face counter
  coarseI = 0;
  FOR_ALL(oldPatches, patchI) {
    patchStarts[patchI] = coarseI;
    const polyPatch& pp = oldPatches[patchI];
    if (pp.size() > 0) {
      patchFaceMap_[patchI].setSize(nAgglom[patchI]);
      // Patchfaces per agglomeration
      labelListList agglomToPatch
      {
        invertOneToMany(nAgglom[patchI], agglom[patchI])
      };
      // From agglomeration to compact patch face
      labelList agglomToFace{nAgglom[patchI], -1};
      FOR_ALL(pp, i) {
        label myAgglom = agglom[patchI][i];
        if (agglomToFace[myAgglom] == -1) {
          // Agglomeration not yet done. We now have:
          // - coarseI                  : current coarse mesh face
          // - patchStarts[patchI]      : coarse mesh patch start
          // - myAgglom                 : agglomeration
          // -  agglomToPatch[myAgglom] : fine mesh faces for zone
          label coarsePatchFaceI = coarseI - patchStarts[patchI];
          patchFaceMap_[patchI][coarsePatchFaceI] = myAgglom;
          agglomToFace[myAgglom] = coarsePatchFaceI;
          const labelList& fineFaces = agglomToPatch[myAgglom];
          // Create overall map from fine mesh faces to coarseI.
          FOR_ALL(fineFaces, fineI) {
            reverseFaceMap_[pp.start()+fineFaces[fineI]] = coarseI;
          }
          // Construct single face
          uindirectPrimitivePatch upp
          {
            UIndirectList<face>(pp, fineFaces),
            pp.points()
          };
          if (upp.edgeLoops().size() != 1) {
            FATAL_ERROR_IN
            (
              "singleCellFvMesh::agglomerateMesh(..)"
            )
            << "agglomeration does not create a"
            << " single, non-manifold"
            << " face for agglomeration " << myAgglom
            << " on patch " <<  patchI
            << exit(FatalError);
          }
          patchFaces[coarseI++] = face
          {
            renumber
            (
              upp.meshPoints(),
              upp.edgeLoops()[0]
            )
          };
        }
      }
    }
    patchSizes[patchI] = coarseI - patchStarts[patchI];
  }
  // Compact numbering for points
  reversePointMap_.setSize(mesh.nPoints());
  reversePointMap_.labelList::operator=(-1);
  label newI = 0;
  FOR_ALL(patchFaces, coarseI) {
    face& f = patchFaces[coarseI];
    FOR_ALL(f, fp) {
      if (reversePointMap_[f[fp]] == -1) {
        reversePointMap_[f[fp]] = newI++;
      }
      f[fp] = reversePointMap_[f[fp]];
    }
  }
  pointMap_ = invert(newI, reversePointMap_);
  // Subset used points
  pointField boundaryPoints{mesh.points(), pointMap_};
  // Add patches (on still zero sized mesh)
  List<polyPatch*> newPatches{oldPatches.size()};
  FOR_ALL(oldPatches, patchI) {
    newPatches[patchI] = oldPatches[patchI].clone
    (
      boundaryMesh(),
      patchI,
      0,
      0
    ).ptr();
  }
  addFvPatches(newPatches);
  // Owner, neighbour is trivial
  labelList owner{patchFaces.size(), 0};
  labelList neighbour{0};
  // actually change the mesh
  resetPrimitives
  (
    xferMove(boundaryPoints),
    xferMove(patchFaces),
    xferMove(owner),
    xferMove(neighbour),
    patchSizes,
    patchStarts,
    true                //syncPar
  );
  // Adapt the zones
  cellZones().clear();
  cellZones().setSize(mesh.cellZones().size());

  {
    FOR_ALL(mesh.cellZones(), zoneI) {
      const cellZone& oldFz = mesh.cellZones()[zoneI];
      DynamicList<label> newAddressing;
      //Note: uncomment if you think it makes sense. Note that value
      // of cell0 is the average.
      //// Was old cell0 in this cellZone?
      //if (oldFz.localID(0) != -1)
      //{
      //    newAddressing.append(0);
      //}
      cellZones().set
      (
        zoneI,
        oldFz.clone
        (
          newAddressing,
          zoneI,
          cellZones()
        )
      );
    }
  }
  faceZones().clear();
  faceZones().setSize(mesh.faceZones().size());

  {
    FOR_ALL(mesh.faceZones(), zoneI) {
      const faceZone& oldFz = mesh.faceZones()[zoneI];
      DynamicList<label> newAddressing{oldFz.size()};
      DynamicList<bool> newFlipMap{oldFz.size()};
      FOR_ALL(oldFz, i) {
        label newFaceI = reverseFaceMap_[oldFz[i]];
        if (newFaceI != -1) {
          newAddressing.append(newFaceI);
          newFlipMap.append(oldFz.flipMap()[i]);
        }
      }
      faceZones().set
      (
        zoneI,
        oldFz.clone
        (
          newAddressing,
          newFlipMap,
          zoneI,
          faceZones()
        )
      );
    }
  }
  pointZones().clear();
  pointZones().setSize(mesh.pointZones().size());

  {
    FOR_ALL(mesh.pointZones(), zoneI) {
      const pointZone& oldFz = mesh.pointZones()[zoneI];
      DynamicList<label> newAddressing{oldFz.size()};
      FOR_ALL(oldFz, i) {
        label newPointI  = reversePointMap_[oldFz[i]];
        if (newPointI != -1) {
          newAddressing.append(newPointI);
        }
      }
      pointZones().set
      (
        zoneI,
        oldFz.clone
        (
          pointZones(),
          zoneI,
          newAddressing
        )
      );
    }
  }
}


// Constructors 
mousse::singleCellFvMesh::singleCellFvMesh
(
  const IOobject& io,
  const fvMesh& mesh
)
:
  fvMesh
  {
    io,
    xferCopy(pointField()), //points
    xferCopy(faceList()),   //faces
    xferCopy(labelList()),  //allOwner
    xferCopy(labelList()),  //allNeighbour
    false                   //syncPar
  },
  patchFaceAgglomeration_
  {
    {
      "patchFaceAgglomeration",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    0
  },
  patchFaceMap_
  {
    {
      "patchFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.boundaryMesh().size()
  },
  reverseFaceMap_
  {
    {
      "reverseFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nFaces()
  },
  pointMap_
  {
    {
      "pointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nPoints()
  },
  reversePointMap_
  {
    {
      "reversePointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nPoints()
  }
{
  const polyBoundaryMesh& oldPatches = mesh.boundaryMesh();
  labelListList agglom{oldPatches.size()};
  FOR_ALL(oldPatches, patchI) {
    agglom[patchI] = identity(oldPatches[patchI].size());
  }
  agglomerateMesh(mesh, agglom);
}


mousse::singleCellFvMesh::singleCellFvMesh
(
  const IOobject& io,
  const fvMesh& mesh,
  const labelListList& patchFaceAgglomeration
)
:
  fvMesh
  {
    io,
    xferCopy(pointField()), //points
    xferCopy(faceList()),   //faces
    xferCopy(labelList()),  //allOwner
    xferCopy(labelList()),  //allNeighbour
    false                   //syncPar
  },
  patchFaceAgglomeration_
  {
    {
      "patchFaceAgglomeration",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    patchFaceAgglomeration
  },
  patchFaceMap_
  {
    {
      "patchFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.boundaryMesh().size()
  },
  reverseFaceMap_
  {
    {
      "reverseFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nFaces()
  },
  pointMap_
  {
    {
      "pointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nPoints()
  },
  reversePointMap_
  {
    {
      "reversePointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    },
    mesh.nPoints()
  }
{
  agglomerateMesh(mesh, patchFaceAgglomeration);
}


mousse::singleCellFvMesh::singleCellFvMesh(const IOobject& io)
:
  fvMesh{io},
  patchFaceAgglomeration_
  {
    {
      "patchFaceAgglomeration",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    }
  },
  patchFaceMap_
  {
    {
      "patchFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    }
  },
  reverseFaceMap_
  {
    {
      "reverseFaceMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    }
  },
  pointMap_
  {
    {
      "pointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    }
  },
  reversePointMap_
  {
    {
      "reversePointMap",
      io.instance(),
      fvMesh::meshSubDir,
      *this,
      io.readOpt(),
      io.writeOpt()
    }
  }
{}

