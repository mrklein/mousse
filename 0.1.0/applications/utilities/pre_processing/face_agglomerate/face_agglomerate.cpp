// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "compact_list_list.hpp"
#include "unit_conversion.hpp"
#include "pair_patch_agglomeration.hpp"
#include "label_list_io_list.hpp"
#include "sync_tools.hpp"
#include "global_index.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_mesh.inc"
  const word dictName{"viewFactorsDict"};
  #include "set_constant_mesh_dictionary_io.inc"
  // Read control dictionary
  const IOdictionary agglomDict{dictIO};
  bool writeAgglom = readBool(agglomDict.lookup("writeFacesAgglomeration"));
  const polyBoundaryMesh& boundary = mesh.boundaryMesh();
  labelListIOList finalAgglom
  {
    {
      "finalAgglom",
      mesh.facesInstance(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    boundary.size()
  };
  label nCoarseFaces = 0;
  FOR_ALL_CONST_ITER(dictionary, agglomDict, iter) {
    labelList patchIds = boundary.findIndices(iter().keyword());
    FOR_ALL(patchIds, i) {
      label patchI =  patchIds[i];
      const polyPatch& pp = boundary[patchI];
      if (pp.coupled())
        continue;
      Info << "\nAgglomerating patch : " << pp.name() << endl;
      pairPatchAgglomeration agglomObject
      {
        pp,
        agglomDict.subDict(pp.name())
      };
      agglomObject.agglomerate();
      finalAgglom[patchI] = agglomObject.restrictTopBottomAddressing();
      if (finalAgglom[patchI].size()) {
        nCoarseFaces += max(finalAgglom[patchI] + 1);
      }
    }
  }
  // - All patches which are not agglomarated are identity for finalAgglom
  FOR_ALL(boundary, patchId) {
    if (finalAgglom[patchId].size() == 0) {
      finalAgglom[patchId] = identity(boundary[patchId].size());
    }
  }
  // Sync agglomeration across coupled patches
  labelList nbrAgglom{mesh.nFaces() - mesh.nInternalFaces(), -1};
  FOR_ALL(boundary, patchId) {
    const polyPatch& pp = boundary[patchId];
    if (pp.coupled()) {
      finalAgglom[patchId] = identity(pp.size());
      FOR_ALL(pp, i) {
        nbrAgglom[pp.start() - mesh.nInternalFaces() + i] =
          finalAgglom[patchId][i];
      }
    }
  }
  syncTools::swapBoundaryFaceList(mesh, nbrAgglom);
  FOR_ALL(boundary, patchId) {
    const polyPatch& pp = boundary[patchId];
    if (pp.coupled() && !refCast<const coupledPolyPatch>(pp).owner()) {
      FOR_ALL(pp, i) {
        finalAgglom[patchId][i] =
          nbrAgglom[pp.start() - mesh.nInternalFaces() + i];
      }
    }
  }
  finalAgglom.write();
  if (writeAgglom) {
    globalIndex index{nCoarseFaces};
    volScalarField facesAgglomeration
    {
      {
        "facesAgglomeration",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"facesAgglomeration", dimless, 0}
    };
    label coarsePatchIndex = 0;
    FOR_ALL(boundary, patchId) {
      const polyPatch& pp = boundary[patchId];
      if (pp.size() <= 0)
        continue;
      fvPatchScalarField& bFacesAgglomeration =
        facesAgglomeration.boundaryField()[patchId];
      FOR_ALL(bFacesAgglomeration, j) {
        bFacesAgglomeration[j] =
          index.toGlobal
          (
            Pstream::myProcNo(),
            finalAgglom[patchId][j] + coarsePatchIndex
          );
      }
      coarsePatchIndex += max(finalAgglom[patchId]) + 1;
    }
    Info << "\nWriting facesAgglomeration" << endl;
    facesAgglomeration.write();
  }
  Info << "End\n" << endl;
  return 0;
}

