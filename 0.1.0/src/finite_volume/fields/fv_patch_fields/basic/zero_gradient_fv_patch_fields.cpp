// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zero_gradient_fv_patch_fields.hpp"
#include "fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Static Data Members
MAKE_PATCH_FIELDS(zeroGradient);

}  // namespace mousse
