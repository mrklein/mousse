// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef unsorted_meshed_surfaces_fwd_hpp_
#define unsorted_meshed_surfaces_fwd_hpp_
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
