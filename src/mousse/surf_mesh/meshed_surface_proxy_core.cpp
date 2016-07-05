// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface_proxy.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
#define MAKE_SURFACE(surfType, faceType)                                      \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(surfType<faceType>, 0);           \
  DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE                            \
  (                                                                           \
    surfType,                                                                 \
    write,                                                                    \
    fileExtension,                                                            \
    faceType                                                                  \
  );


MAKE_SURFACE(MeshedSurfaceProxy, face)
MAKE_SURFACE(MeshedSurfaceProxy, triFace)

}  // namespace mousse
