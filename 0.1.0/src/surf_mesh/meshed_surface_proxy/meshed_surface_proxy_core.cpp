// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface_proxy.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
#define makeSurface(surfType, faceType)                                       \
  defineNamedTemplateTypeNameAndDebug(surfType<faceType>, 0);               \
  defineTemplatedMemberFunctionSelectionTable                               \
  (                                                                         \
    surfType,                                                             \
    write,                                                                \
    fileExtension,                                                        \
    faceType                                                              \
  );
makeSurface(MeshedSurfaceProxy, face)
makeSurface(MeshedSurfaceProxy, triFace)
}  // namespace mousse
