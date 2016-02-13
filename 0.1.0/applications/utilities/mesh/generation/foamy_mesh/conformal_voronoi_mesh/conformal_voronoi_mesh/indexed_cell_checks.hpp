// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef indexed_cell_checks_hpp_
#define indexed_cell_checks_hpp_
namespace mousse
{
namespace foamyHexMeshChecks
{
  template<typename Cell>
  scalar coplanarTet
  (
    Cell& c,
    const scalar tol = 1e-12
  );
  template<typename Cell>
  bool closePoints
  (
    Cell& c,
    const scalar tol = 1e-12
  );
  template<typename Cell>
  bool smallVolume
  (
    Cell& c,
    const scalar tol = 0.0
  );
}  // namespace foamyHexMeshChecks
}  // namespace mousse
#ifdef NoRepository
  #include "indexed_cell_checks.cpp"
#endif
#endif
