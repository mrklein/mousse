// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "unsorted_meshed_surfaces.hpp"


namespace mousse {

// Static Data Members
#define MAKE_SURFACE(surfType, faceType)                                      \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(surfType<faceType>, 0);           \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE(surfType,fileExtension,faceType); \
  DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE                            \
  (                                                                           \
    surfType,                                                                 \
    write,                                                                    \
    fileExtension,                                                            \
    faceType                                                                  \
  );

MAKE_SURFACE(UnsortedMeshedSurface, face)
MAKE_SURFACE(UnsortedMeshedSurface, triFace)

}  // namespace mousse
