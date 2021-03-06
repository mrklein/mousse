// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "single_cell_fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "distributed_tri_surface_mesh.hpp"
#include "cyclic_ami_poly_patch.hpp"
#include "map_distribute.hpp"
#include "mesh_tools.hpp"
#include "uindirect_primitive_patch.hpp"
#include "dynamic_field.hpp"
#include "scalar_matrices.hpp"
#include "scalar_list_io_list.hpp"


using namespace mousse;


triSurface triangulate
(
  const polyBoundaryMesh& bMesh,
  const labelHashSet& includePatches,
  const labelListIOList& finalAgglom,
  labelList& triSurfaceToAgglom,
  const globalIndex& globalNumbering,
  const polyBoundaryMesh& coarsePatches
)
{
  const polyMesh& mesh = bMesh.mesh();
  // Storage for surfaceMesh. Size estimate.
  DynamicList<labelledTri> triangles
  {
    mesh.nFaces() - mesh.nInternalFaces()
  };
  label newPatchI = 0;
  label localTriFaceI = 0;
  FOR_ALL_CONST_ITER(labelHashSet, includePatches, iter) {
    const label patchI = iter.key();
    const polyPatch& patch = bMesh[patchI];
    const pointField& points = patch.points();
    label nTriTotal = 0;
    FOR_ALL(patch, patchFaceI) {
      const face& f = patch[patchFaceI];
      faceList triFaces{f.nTriangles(points)};
      label nTri = 0;
      f.triangles(points, nTri, triFaces);
      FOR_ALL(triFaces, triFaceI) {
        const face& f = triFaces[triFaceI];
        triangles.append(labelledTri(f[0], f[1], f[2], newPatchI));
        nTriTotal++;
        triSurfaceToAgglom[localTriFaceI++] =
          globalNumbering.toGlobal
          (
            Pstream::myProcNo(),
            finalAgglom[patchI][patchFaceI] + coarsePatches[patchI].start()
          );
      }
    }
    newPatchI++;
  }
  triSurfaceToAgglom.resize(localTriFaceI);
  triangles.shrink();
  // Create globally numbered tri surface
  triSurface rawSurface{triangles, mesh.points()};
  // Create locally numbered tri surface
  triSurface surface
  {
    rawSurface.localFaces(),
    rawSurface.localPoints()
  };
  // Add patch names to surface
  surface.patches().setSize(newPatchI);
  newPatchI = 0;
  FOR_ALL_CONST_ITER(labelHashSet, includePatches, iter) {
    const label patchI = iter.key();
    const polyPatch& patch = bMesh[patchI];
    surface.patches()[newPatchI].index() = patchI;
    surface.patches()[newPatchI].name() = patch.name();
    surface.patches()[newPatchI].geometricType() = patch.type();
    newPatchI++;
  }
  return surface;
}


void writeRays
(
  const fileName& fName,
  const pointField& compactCf,
  const pointField& myFc,
  const labelListList& visibleFaceFaces
)
{
  OFstream str{fName};
  label vertI = 0;
  Pout << "Dumping rays to " << str.name() << endl;
  FOR_ALL(myFc, faceI) {
    const labelList visFaces = visibleFaceFaces[faceI];
    FOR_ALL(visFaces, faceRemote) {
      label compactI = visFaces[faceRemote];
      const point& remoteFc = compactCf[compactI];
      meshTools::writeOBJ(str, myFc[faceI]);
      vertI++;
      meshTools::writeOBJ(str, remoteFc);
      vertI++;
      str << "l " << vertI-1 << ' ' << vertI << nl;
    }
  }
  string cmd{"objToVTK " + fName + " " + fName.lessExt() + ".vtk"};
  Pout << "cmd:" << cmd << endl;
  system(cmd);
}


scalar calculateViewFactorFij
(
  const vector& i,
  const vector& j,
  const vector& dAi,
  const vector& dAj
)
{
  using constant::mathematical::pi;
  vector r = i - j;
  scalar rMag = mag(r);
  if (rMag > SMALL) {
    scalar dAiMag = mag(dAi);
    scalar dAjMag = mag(dAj);
    vector ni = dAi/dAiMag;
    vector nj = dAj/dAjMag;
    scalar cosThetaJ = mag(nj & r)/rMag;
    scalar cosThetaI = mag(ni & r)/rMag;
    return (cosThetaI*cosThetaJ*dAjMag*dAiMag)/(sqr(rMag)*pi);
  }
  return 0;
}


void insertMatrixElements
(
  const globalIndex& globalNumbering,
  const label fromProcI,
  const labelListList& globalFaceFaces,
  const scalarListList& viewFactors,
  scalarSquareMatrix& matrix
)
{
  FOR_ALL(viewFactors, faceI) {
    const scalarList& vf = viewFactors[faceI];
    const labelList& globalFaces = globalFaceFaces[faceI];
    label globalI = globalNumbering.toGlobal(fromProcI, faceI);
    FOR_ALL(globalFaces, i) {
      matrix[globalI][globalFaces[i]] = vf[i];
    }
  }
}


int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_mesh.inc"
  // Read view factor dictionary
  IOdictionary viewFactorDict
  {
    {
      "viewFactorsDict",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  const bool writeViewFactors =
    viewFactorDict.lookupOrDefault<bool>("writeViewFactorMatrix", false);
  const bool dumpRays =
    viewFactorDict.lookupOrDefault<bool>("dumpRays", false);
  const label debug = viewFactorDict.lookupOrDefault<label>("debug", 0);
  volScalarField Qr
  {
    {
      "Qr",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    mesh
  };
  // Read agglomeration map
  labelListIOList finalAgglom
  {
    {
      "finalAgglom",
      mesh.facesInstance(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  // Create the coarse mesh  using agglomeration
  if (debug) {
    Pout << "\nCreating single cell mesh..." << endl;
  }
  singleCellFvMesh coarseMesh
  {
    {
      "coarse:" + mesh.name(),
      runTime.timeName(),
      runTime,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    finalAgglom
  };
  if (debug) {
    Pout << "\nCreated single cell mesh..." << endl;
  }
  // Calculate total number of fine and coarse faces
  label nCoarseFaces = 0;      //total number of coarse faces
  label nFineFaces = 0;        //total number of fine faces
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  const polyBoundaryMesh& coarsePatches = coarseMesh.boundaryMesh();
  labelList viewFactorsPatches{patches.size()};
  const volScalarField::GeometricBoundaryField& Qrb = Qr.boundaryField();
  label count = 0;
  FOR_ALL(Qrb, patchI) {
    const polyPatch& pp = patches[patchI];
    const fvPatchScalarField& QrpI = Qrb[patchI];
    if ((isA<fixedValueFvPatchScalarField>(QrpI)) && (pp.size() > 0)) {
      viewFactorsPatches[count] = QrpI.patch().index();
      nCoarseFaces += coarsePatches[patchI].size();
      nFineFaces += patches[patchI].size();
      count ++;
    }
  }
  viewFactorsPatches.resize(count);
  // total number of coarse faces
  label totalNCoarseFaces = nCoarseFaces;
  reduce(totalNCoarseFaces, sumOp<label>());
  if (Pstream::master()) {
    Info << "\nTotal number of coarse faces: "<< totalNCoarseFaces << endl;
  }
  if (Pstream::master() && debug) {
    Pout << "\nView factor patches included in the calculation : "
      << viewFactorsPatches << endl;
  }
  // Collect local Cf and Sf on coarse mesh
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DynamicList<point> localCoarseCf{nCoarseFaces};
  DynamicList<point> localCoarseSf{nCoarseFaces};
  DynamicList<label> localAgg{nCoarseFaces};
  FOR_ALL(viewFactorsPatches, i) {
    const label patchID = viewFactorsPatches[i];
    const polyPatch& pp = patches[patchID];
    const labelList& agglom = finalAgglom[patchID];
    label nAgglom = max(agglom) + 1;
    labelListList coarseToFine{invertOneToMany(nAgglom, agglom)};
    const labelList& coarsePatchFace = coarseMesh.patchFaceMap()[patchID];
    const pointField& coarseCf = coarseMesh.Cf().boundaryField()[patchID];
    const pointField& coarseSf = coarseMesh.Sf().boundaryField()[patchID];
    labelHashSet includePatches;
    includePatches.insert(patchID);
    FOR_ALL(coarseCf, faceI) {
      point cf = coarseCf[faceI];
      const label coarseFaceI = coarsePatchFace[faceI];
      const labelList& fineFaces = coarseToFine[coarseFaceI];
      const label agglomI =
        agglom[fineFaces[0]] + coarsePatches[patchID].start();
      // Construct single face
      uindirectPrimitivePatch upp
      {
        UIndirectList<face>{pp, fineFaces},
        pp.points()
      };
      List<point> availablePoints
      {
        upp.faceCentres().size() + upp.localPoints().size()
      };
      SubList<point>
      {
        availablePoints,
        upp.faceCentres().size()
      }.assign(upp.faceCentres());
      SubList<point>
      {
        availablePoints,
        upp.localPoints().size(),
        upp.faceCentres().size()
      }.assign(upp.localPoints());
      point cfo = cf;
      scalar dist = GREAT;
      FOR_ALL(availablePoints, iPoint) {
        point cfFine = availablePoints[iPoint];
        if (mag(cfFine-cfo) < dist) {
          dist = mag(cfFine-cfo);
          cf = cfFine;
        }
      }
      point sf = coarseSf[faceI];
      localCoarseCf.append(cf);
      localCoarseSf.append(sf);
      localAgg.append(agglomI);
    }
  }
  // Distribute local coarse Cf and Sf for shooting rays
  List<pointField> remoteCoarseCf{Pstream::nProcs()};
  List<pointField> remoteCoarseSf{Pstream::nProcs()};
  List<labelField> remoteCoarseAgg{Pstream::nProcs()};
  remoteCoarseCf[Pstream::myProcNo()] = localCoarseCf;
  remoteCoarseSf[Pstream::myProcNo()] = localCoarseSf;
  remoteCoarseAgg[Pstream::myProcNo()] = localAgg;
  Pstream::gatherList(remoteCoarseCf);
  Pstream::scatterList(remoteCoarseCf);
  Pstream::gatherList(remoteCoarseSf);
  Pstream::scatterList(remoteCoarseSf);
  Pstream::gatherList(remoteCoarseAgg);
  Pstream::scatterList(remoteCoarseAgg);
  globalIndex globalNumbering{nCoarseFaces};
  // Set up searching engine for obstacles
  #include "searching_engine.inc"
  // Determine rays between coarse face centres
  DynamicList<label> rayStartFace
  {
    static_cast<label>(nCoarseFaces + 0.01*nCoarseFaces)
  };
  DynamicList<label> rayEndFace{rayStartFace.size()};
  // Return rayStartFace in local index andrayEndFace in global index
  #include "shoot_rays.inc"
  // Calculate number of visible faces from local index
  labelList nVisibleFaceFaces{nCoarseFaces, 0};
  FOR_ALL(rayStartFace, i) {
    nVisibleFaceFaces[rayStartFace[i]]++;
  }
  labelListList visibleFaceFaces{nCoarseFaces};
  label nViewFactors = 0;
  FOR_ALL(nVisibleFaceFaces, faceI) {
    visibleFaceFaces[faceI].setSize(nVisibleFaceFaces[faceI]);
    nViewFactors += nVisibleFaceFaces[faceI];
  }
  // - Construct compact numbering
  // - return map from remote to compact indices
  //   (per processor (!= myProcNo) a map from remote index to compact index)
  // - construct distribute map
  // - renumber rayEndFace into compact addressing
  List<Map<label>> compactMap{Pstream::nProcs()};
  mapDistribute map{globalNumbering, rayEndFace, compactMap};
  labelListIOList IOsubMap
  {
    {
      "subMap",
      mesh.facesInstance(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    map.subMap()
  };
  IOsubMap.write();
  labelListIOList IOconstructMap
  {
    {
      "constructMap",
      mesh.facesInstance(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    map.constructMap()
  };
  IOconstructMap.write();
  IOList<label> consMapDim
  {
    {
      "constructMapDim",
      mesh.facesInstance(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    List<label>{1, map.constructSize()}
  };
  consMapDim.write();
  // visibleFaceFaces has:
  //    (local face, local viewed face) = compact viewed face
  nVisibleFaceFaces = 0;
  FOR_ALL(rayStartFace, i)
  {
    label faceI = rayStartFace[i];
    label compactI = rayEndFace[i];
    visibleFaceFaces[faceI][nVisibleFaceFaces[faceI]++] = compactI;
  }
  // Construct data in compact addressing
  // I need coarse Sf (Ai), fine Sf (dAi) and fine Cf(r) to calculate Fij
  pointField compactCoarseCf{map.constructSize(), pTraits<vector>::zero};
  pointField compactCoarseSf{map.constructSize(), pTraits<vector>::zero};
  List<List<point>> compactFineSf{map.constructSize()};
  List<List<point>> compactFineCf{map.constructSize()};
  DynamicList<label> compactPatchId{map.constructSize()};
  // Insert my coarse local values
  SubList<point>{compactCoarseSf, nCoarseFaces}.assign(localCoarseSf);
  SubList<point>{compactCoarseCf, nCoarseFaces}.assign(localCoarseCf);
  // Insert my fine local values
  label compactI = 0;
  FOR_ALL(viewFactorsPatches, i) {
    label patchID = viewFactorsPatches[i];
    const labelList& agglom = finalAgglom[patchID];
    label nAgglom = max(agglom)+1;
    labelListList coarseToFine{invertOneToMany(nAgglom, agglom)};
    const labelList& coarsePatchFace = coarseMesh.patchFaceMap()[patchID];
    FOR_ALL(coarseToFine, coarseI) {
      compactPatchId.append(patchID);
      List<point>& fineCf = compactFineCf[compactI];
      List<point>& fineSf = compactFineSf[compactI++];
      const label coarseFaceI = coarsePatchFace[coarseI];
      const labelList& fineFaces = coarseToFine[coarseFaceI];
      fineCf.setSize(fineFaces.size());
      fineSf.setSize(fineFaces.size());
      fineCf =
        UIndirectList<point>
        {
          mesh.Cf().boundaryField()[patchID],
          coarseToFine[coarseFaceI]
        };
      fineSf =
        UIndirectList<point>
        {
          mesh.Sf().boundaryField()[patchID],
          coarseToFine[coarseFaceI]
        };
    }
  }
  // Do all swapping
  map.distribute(compactCoarseSf);
  map.distribute(compactCoarseCf);
  map.distribute(compactFineCf);
  map.distribute(compactFineSf);
  map.distribute(compactPatchId);
  // Plot all rays between visible faces.
  if (dumpRays) {
    writeRays
    (
      runTime.path()/"allVisibleFaces.obj",
      compactCoarseCf,
      remoteCoarseCf[Pstream::myProcNo()],
      visibleFaceFaces
    );
  }
  // Fill local view factor matrix
  scalarListIOList F
  {
    {
      "F",
      mesh.facesInstance(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    nCoarseFaces
  };
  label totalPatches = coarsePatches.size();
  reduce(totalPatches, maxOp<label>());
  // Matrix sum in j(Fij) for each i (if enclosure sum = 1)
  scalarSquareMatrix sumViewFactorPatch
  {
    totalPatches,
    totalPatches,
    0.0
  };
  scalarList patchArea{totalPatches, 0.0};
  if (Pstream::master()) {
    Info << "\nCalculating view factors..." << endl;
  }
  if (mesh.nSolutionD() == 3) {
    FOR_ALL(localCoarseSf, coarseFaceI) {
      const List<point>& localFineSf = compactFineSf[coarseFaceI];
      const vector Ai = sum(localFineSf);
      const List<point>& localFineCf = compactFineCf[coarseFaceI];
      const label fromPatchId = compactPatchId[coarseFaceI];
      patchArea[fromPatchId] += mag(Ai);
      const labelList& visCoarseFaces = visibleFaceFaces[coarseFaceI];
      FOR_ALL(visCoarseFaces, visCoarseFaceI) {
        F[coarseFaceI].setSize(visCoarseFaces.size());
        label compactJ = visCoarseFaces[visCoarseFaceI];
        const List<point>& remoteFineSj = compactFineSf[compactJ];
        const List<point>& remoteFineCj = compactFineCf[compactJ];
        const label toPatchId = compactPatchId[compactJ];
        scalar Fij = 0;
        FOR_ALL(localFineSf, i) {
          const vector& dAi = localFineSf[i];
          const vector& dCi = localFineCf[i];
          FOR_ALL(remoteFineSj, j) {
            const vector& dAj = remoteFineSj[j];
            const vector& dCj = remoteFineCj[j];
            scalar dIntFij = calculateViewFactorFij(dCi, dCj, dAi, dAj);
            Fij += dIntFij;
          }
        }
        F[coarseFaceI][visCoarseFaceI] = Fij/mag(Ai);
        sumViewFactorPatch[fromPatchId][toPatchId] += Fij;
      }
    }
  } else if (mesh.nSolutionD() == 2) {
    const boundBox& box = mesh.bounds();
    const Vector<label>& dirs = mesh.geometricD();
    vector emptyDir = vector::zero;
    FOR_ALL(dirs, i) {
      if (dirs[i] == -1) {
        emptyDir[i] = 1.0;
      }
    }
    scalar wideBy2 = (box.span() & emptyDir)*2.0;
    FOR_ALL(localCoarseSf, coarseFaceI) {
      const vector& Ai = localCoarseSf[coarseFaceI];
      const vector& Ci = localCoarseCf[coarseFaceI];
      vector Ain = Ai/mag(Ai);
      vector R1i = Ci + (mag(Ai)/wideBy2)*(Ain ^ emptyDir);
      vector R2i = Ci - (mag(Ai)/wideBy2)*(Ain ^ emptyDir) ;
      const label fromPatchId = compactPatchId[coarseFaceI];
      patchArea[fromPatchId] += mag(Ai);
      const labelList& visCoarseFaces = visibleFaceFaces[coarseFaceI];
      FOR_ALL(visCoarseFaces, visCoarseFaceI) {
        F[coarseFaceI].setSize(visCoarseFaces.size());
        label compactJ = visCoarseFaces[visCoarseFaceI];
        const vector& Aj = compactCoarseSf[compactJ];
        const vector& Cj = compactCoarseCf[compactJ];
        const label toPatchId = compactPatchId[compactJ];
        vector Ajn = Aj/mag(Aj);
        vector R1j = Cj + (mag(Aj)/wideBy2)*(Ajn ^ emptyDir);
        vector R2j = Cj - (mag(Aj)/wideBy2)*(Ajn ^ emptyDir);
        scalar d1 = mag(R1i - R2j);
        scalar d2 = mag(R2i - R1j);
        scalar s1 = mag(R1i - R1j);
        scalar s2 = mag(R2i - R2j);
        scalar Fij = mag((d1 + d2) - (s1 + s2))/(4.0*mag(Ai)/wideBy2);
        F[coarseFaceI][visCoarseFaceI] = Fij;
        sumViewFactorPatch[fromPatchId][toPatchId] += Fij*mag(Ai);
      }
    }
  }
  if (Pstream::master()) {
    Info << "Writing view factor matrix..." << endl;
  }
  // Write view factors matrix in listlist form
  F.write();
  reduce(sumViewFactorPatch, sumOp<scalarSquareMatrix>());
  reduce(patchArea, sumOp<scalarList>());
  if (Pstream::master() && debug) {
    FOR_ALL(viewFactorsPatches, i) {
      label patchI =  viewFactorsPatches[i];
      FOR_ALL(viewFactorsPatches, i) {
        label patchJ =  viewFactorsPatches[i];
        Info << "F" << patchI << patchJ << ": "
          << sumViewFactorPatch[patchI][patchJ]/patchArea[patchI]
          << endl;
      }
    }
  }
  if (writeViewFactors) {
    volScalarField viewFactorField
    {
      {
        "viewFactorField",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"viewFactorField", dimless, 0}
    };
    label compactI = 0;
    FOR_ALL(viewFactorsPatches, i) {
      label patchID = viewFactorsPatches[i];
      const labelList& agglom = finalAgglom[patchID];
      label nAgglom = max(agglom)+1;
      labelListList coarseToFine{invertOneToMany(nAgglom, agglom)};
      const labelList& coarsePatchFace = coarseMesh.patchFaceMap()[patchID];
      FOR_ALL(coarseToFine, coarseI) {
        const scalar Fij = sum(F[compactI]);
        const label coarseFaceID = coarsePatchFace[coarseI];
        const labelList& fineFaces = coarseToFine[coarseFaceID];
        FOR_ALL(fineFaces, fineId) {
          const label faceID = fineFaces[fineId];
          viewFactorField.boundaryField()[patchID][faceID] = Fij;
        }
        compactI++;
      }
    }
    viewFactorField.write();
  }
  // Invert compactMap (from processor+localface to compact) to go
  // from compact to processor+localface (expressed as a globalIndex)
  // globalIndex globalCoarFaceNum(coarseMesh.nFaces());
  labelList compactToGlobal{map.constructSize()};
  // Local indices first (note: are not in compactMap)
  for (label i = 0; i < globalNumbering.localSize(); i++) {
    compactToGlobal[i] = globalNumbering.toGlobal(i);
  }
  FOR_ALL(compactMap, procI) {
    const Map<label>& localToCompactMap = compactMap[procI];
    FOR_ALL_CONST_ITER(Map<label>, localToCompactMap, iter) {
      compactToGlobal[iter()] = globalNumbering.toGlobal(procI, iter.key());
    }
  }
  if (Pstream::master()) {
    scalarSquareMatrix Fmatrix{totalNCoarseFaces, totalNCoarseFaces, 0.0};
    labelListList globalFaceFaces{visibleFaceFaces.size()};
    // Create globalFaceFaces needed to insert view factors
    // in F to the global matrix Fmatrix
    FOR_ALL(globalFaceFaces, faceI) {
      globalFaceFaces[faceI] =
        renumber(compactToGlobal, visibleFaceFaces[faceI]);
    }
    labelListIOList IOglobalFaceFaces
    {
      {
        "globalFaceFaces",
        mesh.facesInstance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      globalFaceFaces
    };
    IOglobalFaceFaces.write();
  } else {
    labelListList globalFaceFaces{visibleFaceFaces.size()};
    FOR_ALL(globalFaceFaces, faceI) {
      globalFaceFaces[faceI] =
        renumber(compactToGlobal, visibleFaceFaces[faceI]);
    }
    labelListIOList IOglobalFaceFaces
    {
      {
        "globalFaceFaces",
        mesh.facesInstance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      globalFaceFaces
    };
    IOglobalFaceFaces.write();
  }
  Info << "End\n" << endl;
  return 0;
}

