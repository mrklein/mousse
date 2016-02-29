#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_PARTICLE_POSITIONS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_PARTICLE_POSITIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ensight_particle_positions.cpp
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
