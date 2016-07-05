// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read MeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  TRIsurfaceFormat,
  face,
  fileExtension,
  tri
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  TRIsurfaceFormat,
  triFace,
  fileExtension,
  tri
);
// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  TRIsurfaceFormat,
  face,
  write,
  fileExtension,
  tri
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  TRIsurfaceFormat,
  triFace,
  write,
  fileExtension,
  tri
);
// write UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  TRIsurfaceFormat,
  face,
  write,
  fileExtension,
  tri
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  TRIsurfaceFormat,
  triFace,
  write,
  fileExtension,
  tri
);

}
}
