// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh_subset.hpp"
#include "arg_list.hpp"
#include "cell_set.hpp"
#include "ioobject_list.hpp"
#include "vol_fields.hpp"
#include "map_poly_mesh.hpp"
#include "face_set.hpp"
#include "cell_set.hpp"
#include "sync_tools.hpp"
#include "poly_topo_change.hpp"
#include "poly_modify_face.hpp"
#include "poly_add_face.hpp"
#include "region_split.hpp"
#include "tuple2.hpp"
#include "cyclic_fv_patch.hpp"


using namespace mousse;


void modifyOrAddFace
(
  polyTopoChange& meshMod,
  const face& f,
  const label faceI,
  const label own,
  const bool flipFaceFlux,
  const label newPatchI,
  const label zoneID,
  const bool zoneFlip,
  PackedBoolList& modifiedFace
)
{
  if (!modifiedFace[faceI]) {
    // First usage of face. Modify.
    meshMod.setAction
      (
        polyModifyFace
        {
          f,                          // modified face
          faceI,                      // label of face
          own,                        // owner
          -1,                         // neighbour
          flipFaceFlux,               // face flip
          newPatchI,                  // patch for face
          false,                      // remove from zone
          zoneID,                     // zone for face
          zoneFlip                    // face flip in zone
        }
      );
    modifiedFace[faceI] = 1;
  } else {
    // Second or more usage of face. Add.
    meshMod.setAction
      (
        polyAddFace
        {
          f,                          // modified face
          own,                        // owner
          -1,                         // neighbour
          -1,                         // master point
          -1,                         // master edge
          faceI,                      // master face
          flipFaceFlux,               // face flip
          newPatchI,                  // patch for face
          zoneID,                     // zone for face
          zoneFlip                    // face flip in zone
        }
      );
  }
}


template<class Type>
void subsetVolFields
(
  const fvMeshSubset& subsetter,
  const IOobjectList& objectsList,
  const label patchI,
  const Type& exposedValue,
  const word GeomVolType,
  PtrList<GeometricField<Type, fvPatchField, volMesh> >& subFields
)
{
  const fvMesh& baseMesh = subsetter.baseMesh();
  label i = 0;
  FOR_ALL_CONST_ITER(IOobjectList , objectsList, iter) {
    if (iter()->headerClassName() != GeomVolType)
      continue;
    const word fieldName = iter()->name();
    Info << "Subsetting field " << fieldName << endl;
    GeometricField<Type, fvPatchField, volMesh> volField{*iter(), baseMesh};
    subFields.set(i, subsetter.interpolate(volField));
    // Explicitly set exposed faces (in patchI) to exposedValue.
    if (patchI < 0)
      continue;
    fvPatchField<Type>& fld = subFields[i++].boundaryField()[patchI];
    label newStart = fld.patch().patch().start();
    label oldPatchI = subsetter.patchMap()[patchI];
    if (oldPatchI == -1) {
      // New patch. Reset whole value.
      fld = exposedValue;
    } else {
      // Reset those faces that originate from different patch
      // or internal faces.
      label oldSize = volField.boundaryField()[oldPatchI].size();
      label oldStart =
        volField.boundaryField()[oldPatchI].patch().patch().start();
      FOR_ALL(fld, j) {
        label oldFaceI = subsetter.faceMap()[newStart+j];
        if (oldFaceI < oldStart || oldFaceI >= oldStart+oldSize) {
          fld[j] = exposedValue;
        }
      }
    }
  }
}


template<class Type>
void subsetSurfaceFields
(
  const fvMeshSubset& subsetter,
  const IOobjectList& objectsList,
  const label patchI,
  const Type& exposedValue,
  const word GeomSurfType,
  PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& subFields
)
{
  const fvMesh& baseMesh = subsetter.baseMesh();
  label i{0};
  FOR_ALL_CONST_ITER(IOobjectList , objectsList, iter) {
    if (iter()->headerClassName() != GeomSurfType)
      continue;
    const word& fieldName = iter.key();
    Info << "Subsetting field " << fieldName << endl;
    GeometricField<Type, fvsPatchField, surfaceMesh> volField
    {
      *iter(),
      baseMesh
    };
    subFields.set(i, subsetter.interpolate(volField));
    // Explicitly set exposed faces (in patchI) to exposedValue.
    if (patchI < 0)
      continue;
    fvsPatchField<Type>& fld = subFields[i++].boundaryField()[patchI];
    label newStart = fld.patch().patch().start();
    label oldPatchI = subsetter.patchMap()[patchI];
    if (oldPatchI == -1) {
      // New patch. Reset whole value.
      fld = exposedValue;
    } else {
      // Reset those faces that originate from different patch
      // or internal faces.
      label oldSize = volField.boundaryField()[oldPatchI].size();
      label oldStart =
        volField.boundaryField()[oldPatchI].patch().patch().start();
      FOR_ALL(fld, j) {
        label oldFaceI = subsetter.faceMap()[newStart+j];
        if (oldFaceI < oldStart || oldFaceI >= oldStart+oldSize) {
          fld[j] = exposedValue;
        }
      }
    }
  }
}


// Faces introduced into zero-sized patches don't get a value at all.
// This is hack to set them to an initial value.
template<class GeoField>
void initCreatedPatches
(
  const fvMesh& mesh,
  const mapPolyMesh& map,
  const typename GeoField::value_type initValue
)
{
  HashTable<const GeoField*> fields
  {
    mesh.objectRegistry::lookupClass<GeoField>()
  };
  typedef typename HashTable<const GeoField*>::iterator iterator;
  for (iterator fieldIter = fields.begin();
       fieldIter != fields.end();
       ++fieldIter) {
    GeoField& field = const_cast<GeoField&>(*fieldIter());
    FOR_ALL(field.boundaryField(), patchi) {
      if (map.oldPatchSizes()[patchi] != 0)
        continue;
      // Not mapped.
      field.boundaryField()[patchi] = initValue;
      if (field.boundaryField()[patchi].fixesValue())
      {
        field.boundaryField()[patchi] == initValue;
      }
    }
  }
}


void createCoupledBaffles
(
  fvMesh& mesh,
  const labelList& coupledWantedPatch,
  polyTopoChange& meshMod,
  PackedBoolList&  modifiedFace
)
{
  const faceZoneMesh& faceZones = mesh.faceZones();
  FOR_ALL(coupledWantedPatch, faceI) {
    if (coupledWantedPatch[faceI] == -1)
      continue;
    const face& f = mesh.faces()[faceI];
    label zoneID = faceZones.whichZone(faceI);
    bool zoneFlip = false;
    if (zoneID >= 0) {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    // Use owner side of face
    modifyOrAddFace
      (
        meshMod,
        f,                          // modified face
        faceI,                      // label of face
        mesh.faceOwner()[faceI],    // owner
        false,                      // face flip
        coupledWantedPatch[faceI],  // patch for face
        zoneID,                     // zone for face
        zoneFlip,                   // face flip in zone
        modifiedFace                // modify or add status
      );
    if (!mesh.isInternalFace(faceI))
      continue;
    zoneID = faceZones.whichZone(faceI);
    zoneFlip = false;
    if (zoneID >= 0) {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    // Use neighbour side of face
    modifyOrAddFace
      (
        meshMod,
        f.reverseFace(),            // modified face
        faceI,                      // label of face
        mesh.faceNeighbour()[faceI],// owner
        false,                      // face flip
        coupledWantedPatch[faceI],  // patch for face
        zoneID,                     // zone for face
        zoneFlip,                   // face flip in zone
        modifiedFace                // modify or add status
      );
  }
}


void createCyclicCoupledBaffles
(
  fvMesh& mesh,
  const labelList& cyclicMasterPatch,
  const labelList& cyclicSlavePatch,
  polyTopoChange& meshMod,
  PackedBoolList&  modifiedFace
)
{
  const faceZoneMesh& faceZones = mesh.faceZones();
  FOR_ALL(cyclicMasterPatch, faceI) {
    if (cyclicMasterPatch[faceI] == -1)
      continue;
    const face& f = mesh.faces()[faceI];
    label zoneID = faceZones.whichZone(faceI);
    bool zoneFlip = false;
    if (zoneID >= 0) {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    modifyOrAddFace
      (
        meshMod,
        f.reverseFace(),                    // modified face
        faceI,                              // label of face
        mesh.faceNeighbour()[faceI],        // owner
        false,                              // face flip
        cyclicMasterPatch[faceI],           // patch for face
        zoneID,                             // zone for face
        zoneFlip,                           // face flip in zone
        modifiedFace                        // modify or add
      );
  }
  FOR_ALL(cyclicSlavePatch, faceI) {
    if (cyclicSlavePatch[faceI] == -1)
      continue;
    const face& f = mesh.faces()[faceI];
    if (!mesh.isInternalFace(faceI))
      continue;
    label zoneID = faceZones.whichZone(faceI);
    bool zoneFlip = false;
    if (zoneID >= 0)
    {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    // Use owner side of face
    modifyOrAddFace
      (
        meshMod,
        f,                          // modified face
        faceI,                      // label of face
        mesh.faceOwner()[faceI],    // owner
        false,                      // face flip
        cyclicSlavePatch[faceI],    // patch for face
        zoneID,                     // zone for face
        zoneFlip,                   // face flip in zone
        modifiedFace                // modify or add status
      );
  }
}


void createBaffles
(
  fvMesh& mesh,
  const labelList& wantedPatch,
  polyTopoChange& meshMod
)
{
  const faceZoneMesh& faceZones = mesh.faceZones();
  Info << "faceZone:createBaffle " << faceZones << endl;
  FOR_ALL(wantedPatch, faceI) {
    if (wantedPatch[faceI] == -1)
      continue;
    const face& f = mesh.faces()[faceI];
    label zoneID = faceZones.whichZone(faceI);
    bool zoneFlip = false;
    if (zoneID >= 0) {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    meshMod.setAction
      (
        polyModifyFace
        {
          f,                          // modified face
          faceI,                      // label of face
          mesh.faceOwner()[faceI],    // owner
          -1,                         // neighbour
          false,                      // face flip
          wantedPatch[faceI],         // patch for face
          false,                      // remove from zone
          zoneID,                     // zone for face
          zoneFlip                    // face flip in zone
        }
      );
    if (!mesh.isInternalFace(faceI))
      continue;
    zoneID = faceZones.whichZone(faceI);
    zoneFlip = false;
    if (zoneID >= 0) {
      const faceZone& fZone = faceZones[zoneID];
      zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
    }
    meshMod.setAction
      (
        polyAddFace
        {
          f.reverseFace(),            // modified face
          mesh.faceNeighbour()[faceI],// owner
          -1,                         // neighbour
          -1,                         // masterPointID
          -1,                         // masterEdgeID
          faceI,                      // masterFaceID,
          false,                      // face flip
          wantedPatch[faceI],         // patch for face
          zoneID,                     // zone for face
          zoneFlip                    // face flip in zone
        }
      );
  }
}


// Wrapper around find patch. Also makes sure same patch in parallel.
label findPatch(const polyBoundaryMesh& patches, const word& patchName)
{
  label patchI = patches.findPatchID(patchName);
  if (patchI == -1) {
    FATAL_ERROR_IN("findPatch(const polyBoundaryMesh&, const word&)")
      << "Illegal patch " << patchName
      << nl << "Valid patches are " << patches.names()
      << exit(FatalError);
  }
  // Check same patch for all procs
  {
    label newPatch = patchI;
    reduce(newPatch, minOp<label>());
    if (newPatch != patchI) {
      FATAL_ERROR_IN("findPatch(const polyBoundaryMesh&, const word&)")
        << "Patch " << patchName
        << " should have the same patch index on all processors." << nl
        << "On my processor it has index " << patchI
        << " ; on some other processor it has index " << newPatch
        << exit(FatalError);
    }
  }
  return patchI;
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_mesh.inc"
  // Read control dictionary
  // ~~~~~~~~~~~~~~~~~~~~~~~
  IOdictionary dict
  {
    {
      "PDRMeshDict",
      runTime.system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  // Per faceSet the patch to put the baffles into
  const List<Pair<word>> setsAndPatches{dict.lookup("blockedFaces")};
  // Per faceSet the patch to put the coupled baffles into
  DynamicList<FixedList<word, 3>> coupledAndPatches{10};
  const dictionary& functionDicts = dict.subDict("coupledFaces");
  FOR_ALL_CONST_ITER(dictionary, functionDicts, iter)
  {
    // safety:
    if (!iter().isDict()) {
      continue;
    }
    const word& key = iter().keyword();
    const dictionary& dict = iter().dict();
    const word cyclicName = dict.lookup("cyclicMasterPatchName");
    const word wallName = dict.lookup("wallPatchName");
    FixedList<word, 3> nameAndType;
    nameAndType[0] = key;
    nameAndType[1] = wallName;
    nameAndType[2] = cyclicName;
    coupledAndPatches.append(nameAndType);
  }
  FOR_ALL(setsAndPatches, setI) {
    Info << "Faces in faceSet " << setsAndPatches[setI][0]
      << " become baffles in patch " << setsAndPatches[setI][1]
      << endl;
  }
  FOR_ALL(coupledAndPatches, setI) {
    Info << "Faces in faceSet " << coupledAndPatches[setI][0]
      << " become coupled baffles in patch " << coupledAndPatches[setI][1]
      << endl;
  }
  // All exposed faces that are not explicitly marked to be put into a patch
  const word defaultPatch{dict.lookup("defaultPatch")};
  Info << "Faces that get exposed become boundary faces in patch "
    << defaultPatch << endl;
  const word blockedSetName{dict.lookup("blockedCells")};
  Info << "Reading blocked cells from cellSet " << blockedSetName
    << endl;
  const bool overwrite = args.optionFound("overwrite");
  // Read faceSets, lookup patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Check that face sets don't have coincident faces
  labelList wantedPatch{mesh.nFaces(), -1};
  FOR_ALL(setsAndPatches, setI) {
    faceSet fSet{mesh, setsAndPatches[setI][0]};
    label patchI =
      findPatch
      (
        mesh.boundaryMesh(),
        setsAndPatches[setI][1]
      );
    FOR_ALL_CONST_ITER(faceSet, fSet, iter) {
      if (wantedPatch[iter.key()] != -1) {
        FATAL_ERROR_IN(args.executable())
          << "Face " << iter.key()
          << " is in faceSet " << setsAndPatches[setI][0]
          << " destined for patch " << setsAndPatches[setI][1]
          << " but also in patch " << wantedPatch[iter.key()]
          << exit(FatalError);
      }
      wantedPatch[iter.key()] = patchI;
    }
  }
  // Per face the patch for coupled baffle or -1.
  labelList coupledWantedPatch{mesh.nFaces(), -1};
  labelList cyclicWantedPatch_half0{mesh.nFaces(), -1};
  labelList cyclicWantedPatch_half1{mesh.nFaces(), -1};
  FOR_ALL(coupledAndPatches, setI) {
    const polyBoundaryMesh& patches = mesh.boundaryMesh();
    const label cyclicId = findPatch(patches, coupledAndPatches[setI][2]);
    const label cyclicSlaveId =
      findPatch
      (
        patches,
        refCast<const cyclicFvPatch>
        (
          mesh.boundary()[cyclicId]
        ).neighbFvPatch().name()
      );
    faceSet fSet{mesh, coupledAndPatches[setI][0]};
    label patchI = findPatch(patches, coupledAndPatches[setI][1]);
    FOR_ALL_CONST_ITER(faceSet, fSet, iter) {
      if (coupledWantedPatch[iter.key()] != -1) {
        FATAL_ERROR_IN(args.executable())
          << "Face " << iter.key()
          << " is in faceSet " << coupledAndPatches[setI][0]
          << " destined for patch " << coupledAndPatches[setI][1]
          << " but also in patch " << coupledWantedPatch[iter.key()]
          << exit(FatalError);
      }
      coupledWantedPatch[iter.key()] = patchI;
      cyclicWantedPatch_half0[iter.key()] = cyclicId;
      cyclicWantedPatch_half1[iter.key()] = cyclicSlaveId;
    }
  }
  // Exposed faces patch
  label defaultPatchI = findPatch(mesh.boundaryMesh(), defaultPatch);
  // Removing blockedCells (like subsetMesh)
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create mesh subsetting engine
  fvMeshSubset subsetter{mesh};

  {
    cellSet blockedCells{mesh, blockedSetName};
    // invert
    blockedCells.invert(mesh.nCells());
    // Create subsetted mesh.
    subsetter.setLargeCellSubset(blockedCells, defaultPatchI, true);
  }

  // Subset wantedPatch. Note that might also include boundary faces
  // that have been exposed by subsetting.
  wantedPatch = IndirectList<label>{wantedPatch, subsetter.faceMap()}();
  coupledWantedPatch =
    IndirectList<label>
    {
      coupledWantedPatch,
      subsetter.faceMap()
    }();
  cyclicWantedPatch_half0 =
    IndirectList<label>
    {
      cyclicWantedPatch_half0,
      subsetter.faceMap()
    }();
  cyclicWantedPatch_half1 =
    IndirectList<label>
    {
      cyclicWantedPatch_half1,
      subsetter.faceMap()
    }();
  // Read all fields in time and constant directories
  IOobjectList objects{mesh, runTime.timeName()};
  IOobjectList timeObjects{IOobjectList(mesh, mesh.facesInstance())};
  FOR_ALL_CONST_ITER(IOobjectList, timeObjects, iter) {
    if (iter()->headerClassName() == volScalarField::typeName
        || iter()->headerClassName() == volVectorField::typeName
        || iter()->headerClassName() == volSphericalTensorField::typeName
        || iter()->headerClassName() == volTensorField::typeName
        || iter()->headerClassName() == volSymmTensorField::typeName
        || iter()->headerClassName() == surfaceScalarField::typeName
        || iter()->headerClassName() == surfaceVectorField::typeName
        || iter()->headerClassName() == surfaceSphericalTensorField::typeName
        || iter()->headerClassName() == surfaceSymmTensorField::typeName
        || iter()->headerClassName() == surfaceTensorField::typeName) {
      objects.add(*iter());
    }
  }
  // Read vol fields and subset.
  wordList scalarNames{objects.names(volScalarField::typeName)};
  PtrList<volScalarField> scalarFlds{scalarNames.size()};
  subsetVolFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<scalar>::zero,
      volScalarField::typeName,
      scalarFlds
    );
  wordList vectorNames{objects.names(volVectorField::typeName)};
  PtrList<volVectorField> vectorFlds{vectorNames.size()};
  subsetVolFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<vector>::zero,
      volVectorField::typeName,
      vectorFlds
    );
  wordList sphericalTensorNames
  {
    objects.names(volSphericalTensorField::typeName)
  };
  PtrList<volSphericalTensorField> sphericalTensorFlds
  {
    sphericalTensorNames.size()
  };
  subsetVolFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<sphericalTensor>::zero,
      volSphericalTensorField::typeName,
      sphericalTensorFlds
    );
  wordList symmTensorNames{objects.names(volSymmTensorField::typeName)};
  PtrList<volSymmTensorField> symmTensorFlds{symmTensorNames.size()};
  subsetVolFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<symmTensor>::zero,
      volSymmTensorField::typeName,
      symmTensorFlds
    );
  wordList tensorNames{objects.names(volTensorField::typeName)};
  PtrList<volTensorField> tensorFlds{tensorNames.size()};
  subsetVolFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<tensor>::zero,
      volTensorField::typeName,
      tensorFlds
    );
  // Read surface fields and subset.
  wordList surfScalarNames{objects.names(surfaceScalarField::typeName)};
  PtrList<surfaceScalarField> surfScalarFlds{surfScalarNames.size()};
  subsetSurfaceFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<scalar>::zero,
      surfaceScalarField::typeName,
      surfScalarFlds
    );
  wordList surfVectorNames{objects.names(surfaceVectorField::typeName)};
  PtrList<surfaceVectorField> surfVectorFlds{surfVectorNames.size()};
  subsetSurfaceFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<vector>::zero,
      surfaceVectorField::typeName,
      surfVectorFlds
    );
  wordList surfSphericalTensorNames
  {
    objects.names(surfaceSphericalTensorField::typeName)
  };
  PtrList<surfaceSphericalTensorField> surfSphericalTensorFlds
  {
    surfSphericalTensorNames.size()
  };
  subsetSurfaceFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<sphericalTensor>::zero,
      surfaceSphericalTensorField::typeName,
      surfSphericalTensorFlds
    );
  wordList surfSymmTensorNames
  {
    objects.names(surfaceSymmTensorField::typeName)
  };
  PtrList<surfaceSymmTensorField> surfSymmTensorFlds
  {
    surfSymmTensorNames.size()
  };
  subsetSurfaceFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<symmTensor>::zero,
      surfaceSymmTensorField::typeName,
      surfSymmTensorFlds
    );
  wordList surfTensorNames{objects.names(surfaceTensorField::typeName)};
  PtrList<surfaceTensorField> surfTensorFlds{surfTensorNames.size()};
  subsetSurfaceFields
    (
      subsetter,
      objects,
      defaultPatchI,
      pTraits<tensor>::zero,
      surfaceTensorField::typeName,
      surfTensorFlds
    );
  if (!overwrite) {
    runTime++;
  }
  Info << "Writing mesh without blockedCells to time " << runTime.value()
    << endl;
  // Subsetting adds 'subset' prefix. Rename field to be like original.
  FOR_ALL(scalarFlds, i) {
    scalarFlds[i].rename(scalarNames[i]);
    scalarFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(vectorFlds, i) {
    vectorFlds[i].rename(vectorNames[i]);
    vectorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(sphericalTensorFlds, i) {
    sphericalTensorFlds[i].rename(sphericalTensorNames[i]);
    sphericalTensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(symmTensorFlds, i) {
    symmTensorFlds[i].rename(symmTensorNames[i]);
    symmTensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(tensorFlds, i) {
    tensorFlds[i].rename(tensorNames[i]);
    tensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  // Surface ones.
  FOR_ALL(surfScalarFlds, i) {
    surfScalarFlds[i].rename(surfScalarNames[i]);
    surfScalarFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(surfVectorFlds, i) {
    surfVectorFlds[i].rename(surfVectorNames[i]);
    surfVectorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(surfSphericalTensorFlds, i) {
    surfSphericalTensorFlds[i].rename(surfSphericalTensorNames[i]);
    surfSphericalTensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(surfSymmTensorFlds, i) {
    surfSymmTensorFlds[i].rename(surfSymmTensorNames[i]);
    surfSymmTensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  FOR_ALL(surfTensorNames, i) {
    surfTensorFlds[i].rename(surfTensorNames[i]);
    surfTensorFlds[i].writeOpt() = IOobject::AUTO_WRITE;
  }
  subsetter.subMesh().write();
  //
  // Splitting blockedFaces
  // ~~~~~~~~~~~~~~~~~~~~~~
  //
  // Synchronize wantedPatch across coupled patches.
  syncTools::syncFaceList
    (
      subsetter.subMesh(),
      wantedPatch,
      maxEqOp<label>()
    );
  // Synchronize coupledWantedPatch across coupled patches.
  syncTools::syncFaceList
    (
      subsetter.subMesh(),
      coupledWantedPatch,
      maxEqOp<label>()
    );
  // Synchronize cyclicWantedPatch across coupled patches.
  syncTools::syncFaceList
    (
      subsetter.subMesh(),
      cyclicWantedPatch_half0,
      maxEqOp<label>()
    );
  // Synchronize cyclicWantedPatch across coupled patches.
  syncTools::syncFaceList
    (
      subsetter.subMesh(),
      cyclicWantedPatch_half1,
      maxEqOp<label>()
    );
  // Topochange container
  polyTopoChange meshMod{subsetter.subMesh()};
  // Whether first use of face (modify) or consecutive (add)
  PackedBoolList modifiedFace{mesh.nFaces()};
  // Create coupled wall-side baffles
  createCoupledBaffles
    (
      subsetter.subMesh(),
      coupledWantedPatch,
      meshMod,
      modifiedFace
    );
  // Create coupled master/slave cyclic baffles
  createCyclicCoupledBaffles
    (
      subsetter.subMesh(),
      cyclicWantedPatch_half0,
      cyclicWantedPatch_half1,
      meshMod,
      modifiedFace
    );
  // Create wall baffles
  createBaffles
    (
      subsetter.subMesh(),
      wantedPatch,
      meshMod
    );
  if (!overwrite) {
    runTime++;
  }
  // Change the mesh. Change points directly (no inflation).
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(subsetter.subMesh(), false);
  // Update fields
  subsetter.subMesh().updateMesh(map);
  // Fix faces that get mapped to zero-sized patches (these don't get any
  // value)
  initCreatedPatches<volScalarField>(subsetter.subMesh(), map, 0.0);
  initCreatedPatches<volVectorField>(subsetter.subMesh(), map, vector::zero);
  initCreatedPatches<volSphericalTensorField>
    (
      subsetter.subMesh(),
      map,
      sphericalTensor::zero
    );
  initCreatedPatches<volSymmTensorField>
    (
      subsetter.subMesh(),
      map,
      symmTensor::zero
    );
  initCreatedPatches<volTensorField>
    (
      subsetter.subMesh(),
      map,
      tensor::zero
    );
  initCreatedPatches<surfaceScalarField>(subsetter.subMesh(), map, 0.0);
  initCreatedPatches<surfaceVectorField>(subsetter.subMesh(), map, vector::zero);
  initCreatedPatches<surfaceSphericalTensorField>
    (
      subsetter.subMesh(),
      map,
      sphericalTensor::zero
    );
  initCreatedPatches<surfaceSymmTensorField>
    (
      subsetter.subMesh(),
      map,
      symmTensor::zero
    );
  initCreatedPatches<surfaceTensorField>
    (
      subsetter.subMesh(),
      map,
      tensor::zero
    );
  // Move mesh (since morphing might not do this)
  if (map().hasMotionPoints()) {
    subsetter.subMesh().movePoints(map().preMotionPoints());
  }
  Info << "Writing mesh with split blockedFaces to time " << runTime.value()
    << endl;
  subsetter.subMesh().write();
  //
  // Removing inaccessible regions
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Determine connected regions. regionSplit is the labelList with the
  // region per cell.
  regionSplit cellRegion{subsetter.subMesh()};
  if (cellRegion.nRegions() > 1) {
    WARNING_IN(args.executable())
      << "Removing blocked faces and cells created "
      << cellRegion.nRegions()
      << " regions that are not connected via a face." << nl
      << "    This is not supported in solvers." << nl
      << "    Use" << nl << nl
      << "    splitMeshRegions <root> <case> -largestOnly" << nl << nl
      << "    to extract a single region of the mesh." << nl
      << "    This mesh will be written to a new timedirectory"
      << " so might have to be moved back to constant/" << nl
      << endl;
    word startFrom{runTime.controlDict().lookup("startFrom")};
    if (startFrom != "latestTime") {
      WARNING_IN(args.executable())
        << "To run splitMeshRegions please set your"
        << " startFrom entry to latestTime" << endl;
    }
  }
  Info << nl << "End" << endl;
  return 0;
}

