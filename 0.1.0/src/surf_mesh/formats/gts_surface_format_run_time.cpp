// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gts_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  face,
  fileExtension,
  gts
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  triFace,
  fileExtension,
  gts
);
// write MeshedSurface
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurface,
  GTSsurfaceFormat,
  face,
  write,
  fileExtension,
  gts
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurface,
  GTSsurfaceFormat,
  triFace,
  write,
  fileExtension,
  gts
);
// write UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  face,
  write,
  fileExtension,
  gts
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  triFace,
  write,
  fileExtension,
  gts
);

}
}
