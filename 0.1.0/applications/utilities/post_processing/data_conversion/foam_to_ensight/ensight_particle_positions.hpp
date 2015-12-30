// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ensight_particle_positions.cpp
#ifndef ensight_particle_positions_hpp_
#define ensight_particle_positions_hpp_
#include "fv_mesh.hpp"
void ensightParticlePositions
(
  const mousse::fvMesh& mesh,
  const mousse::fileName& postProcPath,
  const mousse::word& timeFile,
  const mousse::word& CloudName,
  const bool dataExists
);
#endif
