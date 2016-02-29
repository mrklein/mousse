#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_PARTS_ENSIGHT_OUTPUT_FUNCTIONS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_PARTS_ENSIGHT_OUTPUT_FUNCTIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ensight_output_functions.cpp
#include "ensight_file.hpp"
#include "cloud.hpp"
#include "poly_mesh.hpp"
#include "ioobject.hpp"
namespace mousse
{
void ensightCaseEntry
(
  OFstream& caseFile,
  const string& ensightType,
  const word& fieldName,
  const fileName& dataMask,
  const fileName& local=fileName::null,
  const label cloudNo=-1,
  const label timeSet=1
);
void ensightParticlePositions
(
  const polyMesh& mesh,
  const fileName& dataDir,
  const fileName& subDir,
  const word& cloudName,
  IOstream::streamFormat format
);
//- Write lagrangian parcels
template<class Type>
void ensightLagrangianField
(
  const IOobject& fieldObject,
  const fileName& dataDir,
  const fileName& subDir,
  const word& cloudName,
  IOstream::streamFormat format
);
//- Write generalized field components
template<class Type>
void ensightVolField
(
  const ensightParts& partsList,
  const IOobject& fieldObject,
  const fvMesh& mesh,
  const fileName& dataDir,
  const fileName& subDir,
  IOstream::streamFormat format
);
} // namespace mousse
#ifdef NoRepository
  #include "ensight_output_functions.cpp"
#endif
#endif
