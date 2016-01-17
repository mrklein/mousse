// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cgal_triangulation_2d_kernel_hpp_
#define cgal_triangulation_2d_kernel_hpp_
#include "cgal/Delaunay_triangulation_2.h"
#ifdef CGAL_INEXACT
  // Fast kernel using a double as the storage type but the triangulation
  // may fail
  #include "cgal/Exact_predicates_inexact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
#else
  // Very robust but expensive kernel
  #include "cgal/Exact_predicates_exact_constructions_kernel.h"
  typedef CGAL::Exact_predicates_exact_constructions_kernel K;
#endif
#endif
