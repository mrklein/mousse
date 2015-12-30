// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef meshed_surfaces_fwd_hpp_
#define meshed_surfaces_fwd_hpp_
namespace mousse
{
class face;
class triFace;
template<class Face> class MeshedSurface;
typedef MeshedSurface<face>    meshedSurface;
typedef MeshedSurface<triFace> triMeshedSurface;
}  // namespace mousse
#endif
