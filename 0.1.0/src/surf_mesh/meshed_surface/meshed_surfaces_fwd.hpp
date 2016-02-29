#ifndef SURF_MESH_MESHED_SURFACE_MESHED_SURFACES_FWD_HPP_
#define SURF_MESH_MESHED_SURFACE_MESHED_SURFACES_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
class face;
class triFace;
template<class Face> class MeshedSurface;
typedef MeshedSurface<face>    meshedSurface;
typedef MeshedSurface<triFace> triMeshedSurface;
}  // namespace mousse
#endif
