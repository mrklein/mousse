// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#if defined(__GNUC__)
#include "surface_fields.hpp"
#endif
#include "cyclic_ami_fvs_patch_fields.hpp"
#include "fvs_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
MAKE_FVS_PATCH_FIELDS(cyclicAMI);

}  // namespace mousse
