#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CGAL_TRIANGULATION_2D_DEFS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CGAL_TRIANGULATION_2D_DEFS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cgal_triangulation_2d_kernel.hpp"
#include "indexed_vertex.hpp"
#include "indexed_face.hpp"


typedef CGAL::indexedVertex<K>                           Vb;
typedef CGAL::indexedFace<K>                             Fb;


#ifdef CGAL_HIERARCHY
  // Data structures for hierarchical Delaunay triangulation which is more
  // efficient but also uses more storage
  #include "CGAL/Triangulation_hierarchy_2.h"
  typedef CGAL::Triangulation_hierarchy_vertex_base_2<Vb> Vbh;
  typedef CGAL::Triangulation_data_structure_2<Vbh, Fb> Tds;
  typedef CGAL::Delaunay_triangulation_2<K, Tds> DT;
  typedef CGAL::Triangulation_hierarchy_2<DT> Delaunay;
#else
  // Data structures for standard Delaunay triangulation
  typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
  typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
#endif

#endif

