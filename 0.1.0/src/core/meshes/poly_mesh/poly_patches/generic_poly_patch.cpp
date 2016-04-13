// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "generic_poly_patch.hpp"
#include "dictionary.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(genericPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, genericPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, genericPolyPatch, dictionary);

}


// Constructors
mousse::genericPolyPatch::genericPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch{name, size, start, index, bm, patchType}
{}


mousse::genericPolyPatch::genericPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch{name, dict, index, bm, patchType},
  actualTypeName_{dict.lookup("type")},
  dict_{dict}
{}


mousse::genericPolyPatch::genericPolyPatch
(
  const genericPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch{pp, bm},
  actualTypeName_{pp.actualTypeName_},
  dict_{pp.dict_}
{}


mousse::genericPolyPatch::genericPolyPatch
(
  const genericPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch{pp, bm, index, newSize, newStart},
  actualTypeName_{pp.actualTypeName_},
  dict_{pp.dict_}
{}


mousse::genericPolyPatch::genericPolyPatch
(
  const genericPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch{pp, bm, index, mapAddressing, newStart},
  actualTypeName_{pp.actualTypeName_},
  dict_{pp.dict_}
{}


// Destructor
mousse::genericPolyPatch::~genericPolyPatch()
{}


// Member Functions
void mousse::genericPolyPatch::write(Ostream& os) const
{
  os.writeKeyword("type") << actualTypeName_ << token::END_STATEMENT << nl;
  patchIdentifier::write(os);
  os.writeKeyword("nFaces") << size() << token::END_STATEMENT << nl;
  os.writeKeyword("startFace") << start() << token::END_STATEMENT << nl;
  FOR_ALL_CONST_ITER(dictionary, dict_, iter) {
    if (iter().keyword() != "type"
        && iter().keyword() != "nFaces"
        && iter().keyword() != "startFace") {
      iter().write(os);
    }
  }
}
