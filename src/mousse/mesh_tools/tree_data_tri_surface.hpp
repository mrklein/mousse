#ifndef MESH_TOOLS_INDEXED_OCTREE_TREE_DATA_TRI_SURFACE_HPP_
#define MESH_TOOLS_INDEXED_OCTREE_TREE_DATA_TRI_SURFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::treeDataTriSurface
// Description
//   Encapsulates data for (indexedOc)tree searches on a triSurface.

#include "tree_data_primitive_patch.hpp"
#include "tri_surface.hpp"
#include "point.hpp"
#include "indexed_octree.hpp"
#include "volume_type.hpp"


namespace mousse {

typedef treeDataPrimitivePatch<triSurface> treeDataTriSurface;
//- Template specialisation of getVolumeType for treeDataTriSurface
template<>
volumeType treeDataPrimitivePatch<triSurface>::getVolumeType
(
  const indexedOctree<treeDataPrimitivePatch<triSurface> >& oc,
  const point& sample
) const;

}
#endif
