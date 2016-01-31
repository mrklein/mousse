// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cgal_triangulation_3d_defs_hpp_
#define cgal_triangulation_3d_defs_hpp_
#include "cgal_triangulation_3d_kernel.hpp"
#include "indexed_vertex.hpp"
#include "indexed_cell.hpp"
typedef CGAL::indexedVertex<K>     Vb;
typedef CGAL::indexedCell<K>       Cb;
typedef CGAL::Compact_location     CompactLocator;
typedef CGAL::Fast_location        FastLocator;
typedef CGAL::Triangulation_data_structure_3<Vb, Cb>           Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds, CompactLocator> Delaunay;
typedef CGAL::Delaunay_triangulation_3<K, Tds, FastLocator>    CellSizeDelaunay;
#endif
