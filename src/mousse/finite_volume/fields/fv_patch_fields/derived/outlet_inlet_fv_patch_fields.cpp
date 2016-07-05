// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "outlet_inlet_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
MAKE_PATCH_FIELDS(outletInlet);

}  // namespace mousse
