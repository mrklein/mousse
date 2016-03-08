#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CGAL_TRIANGULATION_2D_KERNEL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CGAL_TRIANGULATION_2D_KERNEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "CGAL/Delaunay_triangulation_2.h"
#ifdef CGAL_INEXACT
  // Fast kernel using a double as the storage type but the triangulation
  // may fail
  #include "CGAL/Exact_predicates_inexact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
#else
  // Very robust but expensive kernel
  #include "CGAL/Exact_predicates_exact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_exact_constructions_kernel K;
#endif
#endif
