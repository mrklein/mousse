#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_CLOUD_FIELD_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_CLOUD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud.hpp"
#include "ioobject.hpp"


template<class Type>
void ensightCloudField
(
  const IOobject& fieldObject,
  const fileName& postProcPath,
  const word& prepend,
  const label timeIndex,
  const word& timeFile,
  const word& cloudName,
  Ostream& ensightCaseFile,
  const bool dataExists
);

#include "ensight_cloud_field.ipp"

#endif
