#ifndef SURF_MESH_UNSORTED_MESHED_SURFACE_UNSORTED_MESHED_SURFACES_FWD_HPP_
#define SURF_MESH_UNSORTED_MESHED_SURFACE_UNSORTED_MESHED_SURFACES_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
// Forward declaration of classes
class face;
class triFace;
template<class Face> class UnsortedMeshedSurface;
typedef UnsortedMeshedSurface<face>    unsortedMeshedSurface;
typedef UnsortedMeshedSurface<triFace> triUnsortedMeshedSurface;
}  // namespace mousse
#endif
