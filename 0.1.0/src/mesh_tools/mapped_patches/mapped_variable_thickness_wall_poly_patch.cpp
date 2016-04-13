// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_variable_thickness_wall_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map_distribute.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(mappedVariableThicknessWallPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  polyPatch,
  mappedVariableThicknessWallPolyPatch,
  word
);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  polyPatch,
  mappedVariableThicknessWallPolyPatch,
  dictionary
);

}


// Constructors
mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  mappedWallPolyPatch{name, size, start, index, bm, patchType},
  thickness_{size}
{}


mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const word& /*sampleRegion*/,
  const mappedPatchBase::sampleMode /*mode*/,
  const word& /*samplePatch*/,
  const vectorField& /*offset*/,
  const polyBoundaryMesh& bm
)
:
  mappedWallPolyPatch{name, size, start, index, bm, typeName},
  thickness_{size}
{}


mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const word& /*sampleRegion*/,
  const mappedPatchBase::sampleMode /*mode*/,
  const word& /*samplePatch*/,
  const vector& /*offset*/,
  const polyBoundaryMesh& bm
)
:
  mappedWallPolyPatch{name, size, start, index, bm, typeName},
  thickness_{size}
{}


mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  mappedWallPolyPatch{name, dict, index, bm, patchType},
  thickness_{scalarField("thickness", dict, this->size())}
{}


mousse::mappedVariableThicknessWallPolyPatch::
mappedVariableThicknessWallPolyPatch
(
  const mappedVariableThicknessWallPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  mappedWallPolyPatch{pp, bm},
  thickness_{pp.thickness_}
{}


mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const mappedVariableThicknessWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  mappedWallPolyPatch{pp, bm, index, newSize, newStart},
  thickness_{newSize}
{}


mousse::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
  const mappedVariableThicknessWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  mappedWallPolyPatch{pp, bm, index, mapAddressing, newStart},
  thickness_{pp.size()}
{}


// Destructor
mousse::mappedVariableThicknessWallPolyPatch::
~mappedVariableThicknessWallPolyPatch()
{}


// Member Functions
void mousse::mappedVariableThicknessWallPolyPatch::
write(mousse::Ostream& os) const
{
  os.writeKeyword("thickness") << thickness_ << token::END_STATEMENT << nl;
}

