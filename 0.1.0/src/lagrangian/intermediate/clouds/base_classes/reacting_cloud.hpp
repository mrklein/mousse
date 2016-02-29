#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_REACTING_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_REACTING_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingCloud
// Description
//   Virtual abstract base class for templated ReactingCloud
// SourceFiles
//   reacting_cloud.cpp
#include "type_info.hpp"
namespace mousse
{
class reactingCloud
{
public:
  //- Runtime type information
  TYPE_NAME("reactingCloud");
  // Constructors
    //- Null constructor
    reactingCloud();
    //- Disallow default bitwise copy construct
    reactingCloud(const reactingCloud&) = delete;
    //- Disallow default bitwise assignment
    reactingCloud& operator=(const reactingCloud&) = delete;
  //- Destructor
  virtual ~reactingCloud();
};
}  // namespace mousse
#endif
