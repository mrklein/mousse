#ifndef LAGRANGIAN_SPRAY_CLOUDS_BASE_CLASSES_SPRAY_CLOUD_HPP_
#define LAGRANGIAN_SPRAY_CLOUDS_BASE_CLASSES_SPRAY_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sprayCloud
// Description
//   Virtual abstract base class for templated SprayCloud
// SourceFiles
//   spray_cloud.cpp
#include "type_info.hpp"
namespace mousse
{
class sprayCloud
{
public:
  //- Runtime type information
  TYPE_NAME("sprayCloud");
  // Constructors
    //- Null constructor
    sprayCloud();
    //- Disallow default bitwise copy construct
    sprayCloud(const sprayCloud&) = delete;
    //- Disallow default bitwise assignment
    sprayCloud& operator=(const sprayCloud&) = delete;
  //- Destructor
  virtual ~sprayCloud();
};
}  // namespace mousse
#endif
