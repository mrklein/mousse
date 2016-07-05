// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_interaction_data_list.hpp"
#include "string_list_ops.hpp"
#include "empty_poly_patch.hpp"


// Constructors 
mousse::patchInteractionDataList::patchInteractionDataList()
:
  List<patchInteractionData>{},
  patchGroupIDs_{}
{}


mousse::patchInteractionDataList::patchInteractionDataList
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  List<patchInteractionData>{dict.lookup("patches")},
  patchGroupIDs_{this->size()}
{
  const polyBoundaryMesh& bMesh = mesh.boundaryMesh();
  const wordList allPatchNames = bMesh.names();
  const List<patchInteractionData>& items = *this;
  FOR_ALL_REVERSE(items, i) {
    const word& patchName = items[i].patchName();
    labelList patchIDs = findStrings(patchName, allPatchNames);
    if (patchIDs.empty()) {
      WARNING_IN
      (
        "mousse::patchInteractionDataList::patchInteractionDataList"
        "("
          "const polyMesh&, "
          "const dictionary&"
        ")"
      )
      << "Cannot find any patch names matching " << patchName
      << endl;
    }
    patchGroupIDs_[i].transfer(patchIDs);
  }
  // Check that all patches are specified
  DynamicList<word> badPatches;
  FOR_ALL(bMesh, patchI) {
    const polyPatch& pp = bMesh[patchI];
    if (!pp.coupled() && !isA<emptyPolyPatch>(pp)
        && applyToPatch(pp.index()) < 0) {
      badPatches.append(pp.name());
    }
  }
  if (badPatches.size() > 0) {
    FATAL_ERROR_IN
    (
      "mousse::patchInteractionDataList::patchInteractionDataList"
      "("
      "  const polyMesh&, "
      "  const dictionary&"
      ")"
    )
    << "All patches must be specified when employing local patch "
    << "interaction. Please specify data for patches:" << nl
    << badPatches << nl << exit(FatalError);
  }
}


mousse::patchInteractionDataList::patchInteractionDataList
(
  const patchInteractionDataList& pidl
)
:
  List<patchInteractionData>{pidl},
  patchGroupIDs_{pidl.patchGroupIDs_}
{}


// Member Functions 
mousse::label mousse::patchInteractionDataList::applyToPatch(const label id) const
{
  FOR_ALL(patchGroupIDs_, groupI) {
    const labelList& patchIDs = patchGroupIDs_[groupI];
    FOR_ALL(patchIDs, patchI) {
      if (patchIDs[patchI] == id) {
        return groupI;
      }
    }
  }
  return -1;
}

