// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ensight_cloud_field.cpp
#ifndef ensight_cloud_field_hpp_
#define ensight_cloud_field_hpp_
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
#ifdef NoRepository
  #include "ensight_cloud_field.cpp"
#endif
#endif
