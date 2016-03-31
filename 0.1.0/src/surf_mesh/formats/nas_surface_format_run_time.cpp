// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nas_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read MeshedSurface - .bdf (Bulk Data Format)
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  NASsurfaceFormat,
  face,
  fileExtension,
  bdf
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  NASsurfaceFormat,
  face,
  fileExtension,
  nas
);
// read MeshedSurface - .nas (Nastran)
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  NASsurfaceFormat,
  triFace,
  fileExtension,
  bdf
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  NASsurfaceFormat,
  triFace,
  fileExtension,
  nas
);

}
}
