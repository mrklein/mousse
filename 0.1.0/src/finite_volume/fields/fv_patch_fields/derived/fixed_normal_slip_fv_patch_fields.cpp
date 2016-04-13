// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_normal_slip_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Static Data Members
MAKE_TEMPLATE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  fixedNormalSlipFvPatchVectorField
);

MAKE_TEMPLATE_PATCH_TYPE_FIELD
(
  fvPatchTensorField,
  fixedNormalSlipFvPatchTensorField
);

}  // namespace mousse

