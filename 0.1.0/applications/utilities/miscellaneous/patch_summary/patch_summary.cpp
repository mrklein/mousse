// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "ioobject_list.hpp"
#include "patch_summary_templates.hpp"


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  argList::addBoolOption
  (
    "expand",
    "Do not combine patches"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  const bool expand = args.optionFound("expand");
  #include "create_named_mesh.inc"
  const polyBoundaryMesh& bm = mesh.boundaryMesh();
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << nl << endl;
    // Update the mesh if changed
    if (mesh.readUpdate() == polyMesh::TOPO_PATCH_CHANGE) {
      Info << "Detected changed patches. Recreating patch group table."
        << endl;
    }
    const IOobjectList fieldObjs{mesh, runTime.timeName()};
    const wordList objNames = fieldObjs.names();
    PtrList<volScalarField> vsf{objNames.size()};
    PtrList<volVectorField> vvf{objNames.size()};
    PtrList<volSphericalTensorField> vsptf{objNames.size()};
    PtrList<volSymmTensorField> vsytf{objNames.size()};
    PtrList<volTensorField> vtf{objNames.size()};
    PtrList<pointScalarField> psf{objNames.size()};
    PtrList<pointVectorField> pvf{objNames.size()};
    PtrList<pointSphericalTensorField> psptf{objNames.size()};
    PtrList<pointSymmTensorField> psytf{objNames.size()};
    PtrList<pointTensorField> ptf{objNames.size()};
    Info << "Valid fields:" << endl;
    FOR_ALL(objNames, objI) {
      IOobject obj
      {
        objNames[objI],
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      };
      if (obj.headerOk()) {
        addToFieldList(vsf, obj, objI, mesh);
        addToFieldList(vvf, obj, objI, mesh);
        addToFieldList(vsptf, obj, objI, mesh);
        addToFieldList(vsytf, obj, objI, mesh);
        addToFieldList(vtf, obj, objI, mesh);
        addToFieldList(psf, obj, objI, pointMesh::New(mesh));
        addToFieldList(pvf, obj, objI, pointMesh::New(mesh));
        addToFieldList(psptf, obj, objI, pointMesh::New(mesh));
        addToFieldList(psytf, obj, objI, pointMesh::New(mesh));
        addToFieldList(ptf, obj, objI, pointMesh::New(mesh));
      }
    }
    Info << endl;
    if (expand) {
      // Print each patch separately
      FOR_ALL(bm, patchI) {
        Info << bm[patchI].type() << "\t: " << bm[patchI].name() << nl;
        outputFieldList(vsf, patchI);
        outputFieldList(vvf, patchI);
        outputFieldList(vsptf, patchI);
        outputFieldList(vsytf, patchI);
        outputFieldList(vtf, patchI);
        outputFieldList(psf, patchI);
        outputFieldList(pvf, patchI);
        outputFieldList(psptf, patchI);
        outputFieldList(psytf, patchI);
        outputFieldList(ptf, patchI);
        Info << endl;
      }
    } else {
      // Collect for each patch the bc type per field. Merge similar
      // patches.
      // Per 'group', the map from fieldname to patchfield type
      DynamicList<HashTable<word>> fieldToTypes{bm.size()};
      // Per 'group' the patches
      DynamicList<DynamicList<label>> groupToPatches{bm.size()};
      FOR_ALL(bm, patchI) {
        HashTable<word> fieldToType;
        collectFieldList(vsf, patchI, fieldToType);
        collectFieldList(vvf, patchI, fieldToType);
        collectFieldList(vsptf, patchI, fieldToType);
        collectFieldList(vsytf, patchI, fieldToType);
        collectFieldList(vtf, patchI, fieldToType);
        collectFieldList(psf, patchI, fieldToType);
        collectFieldList(pvf, patchI, fieldToType);
        collectFieldList(psptf, patchI, fieldToType);
        collectFieldList(psytf, patchI, fieldToType);
        collectFieldList(ptf, patchI, fieldToType);
        label groupI = findIndex(fieldToTypes, fieldToType);
        if (groupI == -1) {
          DynamicList<label> group{1};
          group.append(patchI);
          groupToPatches.append(group);
          fieldToTypes.append(fieldToType);
        } else {
          groupToPatches[groupI].append(patchI);
        }
      }
      FOR_ALL(groupToPatches, groupI) {
        const DynamicList<label>& patchIDs = groupToPatches[groupI];
        if (patchIDs.size() > 1) {
          // Check if part of a group
          wordList groups;
          labelHashSet nonGroupPatches;
          bm.matchGroups(patchIDs, groups, nonGroupPatches);
          const labelList sortedPatches{nonGroupPatches.sortedToc()};
          FOR_ALL(sortedPatches, i) {
            Info << bm[sortedPatches[i]].type()
              << "\t: " << bm[sortedPatches[i]].name() << nl;
          }
          if (groups.size()) {
            FOR_ALL(groups, i) {
              Info << "group\t: " << groups[i] << nl;
            }
          }
          outputFieldList(vsf, patchIDs[0]);
          outputFieldList(vvf, patchIDs[0]);
          outputFieldList(vsptf, patchIDs[0]);
          outputFieldList(vsytf, patchIDs[0]);
          outputFieldList(vtf, patchIDs[0]);
          outputFieldList(psf, patchIDs[0]);
          outputFieldList(pvf, patchIDs[0]);
          outputFieldList(psptf, patchIDs[0]);
          outputFieldList(psytf, patchIDs[0]);
          outputFieldList(ptf, patchIDs[0]);
          Info << endl;
        } else {
          // No group.
          FOR_ALL(patchIDs, i) {
            label patchI = patchIDs[i];
            Info << bm[patchI].type() << "\t: " << bm[patchI].name() << nl;
            outputFieldList(vsf, patchI);
            outputFieldList(vvf, patchI);
            outputFieldList(vsptf, patchI);
            outputFieldList(vsytf, patchI);
            outputFieldList(vtf, patchI);
            outputFieldList(psf, patchI);
            outputFieldList(pvf, patchI);
            outputFieldList(psptf, patchI);
            outputFieldList(psytf, patchI);
            outputFieldList(ptf, patchI);
            Info<< endl;
          }
        }
      }
    }
  }
  Info << "End\n" << endl;
  return 0;
}

