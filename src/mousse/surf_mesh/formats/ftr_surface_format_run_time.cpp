// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ftr_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  FTRsurfaceFormat,
  face,
  fileExtension,
  ftr
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  FTRsurfaceFormat,
  triFace,
  fileExtension,
  ftr
);

}
}
