#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CGAL_TRIANGULATION_3D_KERNEL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CGAL_TRIANGULATION_3D_KERNEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "CGAL/Delaunay_triangulation_3.h"
#ifdef CGAL_INEXACT
  // Fast kernel using a double as the storage type but the triangulation may
  // fail. Adding robust circumcentre traits.
  #include "CGAL/Exact_predicates_inexact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_inexact_constructions_kernel baseK;
//    #include "cgal/_robust_circumcenter_traits_3.hpp"
//    typedef CGAL::Robust_circumcenter_traits_3<baseK>           K;
  #include "CGAL/Robust_circumcenter_filtered_traits_3.h"
  typedef CGAL::Robust_circumcenter_filtered_traits_3<baseK>  K;
#else
  // Very robust but expensive kernel
  #include "CGAL/Exact_predicates_exact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_exact_constructions_kernel baseK;
  typedef CGAL::Exact_predicates_exact_constructions_kernel K;
#endif
#endif
