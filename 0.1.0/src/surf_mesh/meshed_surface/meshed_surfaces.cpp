// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surfaces.hpp"
namespace mousse
{
// Static Data Members
#define makeSurface(surfType, faceType)                                       \
  defineNamedTemplateTypeNameAndDebug(surfType<faceType>, 0);               \
  defineTemplatedRunTimeSelectionTable(surfType,fileExtension,faceType);    \
  defineTemplatedMemberFunctionSelectionTable                               \
  (                                                                         \
    surfType,                                                             \
    write,                                                                \
    fileExtension,                                                        \
    faceType                                                              \
  );
makeSurface(MeshedSurface, face)
makeSurface(MeshedSurface, triFace)
}  // namespace mousse
