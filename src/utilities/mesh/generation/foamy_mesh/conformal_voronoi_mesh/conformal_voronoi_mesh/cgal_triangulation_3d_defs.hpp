#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CGAL_TRIANGULATION_3D_DEFS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CGAL_TRIANGULATION_3D_DEFS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cgal_triangulation_3d_kernel.hpp"
#include "indexed_vertex.hpp"
#include "indexed_cell.hpp"


typedef CGAL::indexedVertex<K> Vb;
typedef CGAL::indexedCell<K> Cb;
typedef CGAL::Compact_location CompactLocator;
typedef CGAL::Fast_location FastLocator;
typedef CGAL::Triangulation_data_structure_3<Vb, Cb> Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds, CompactLocator> Delaunay;
typedef CGAL::Delaunay_triangulation_3<K, Tds, FastLocator> CellSizeDelaunay;

#endif
