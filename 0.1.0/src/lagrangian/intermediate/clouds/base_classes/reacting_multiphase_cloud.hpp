// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMultiphaseCloud
// Description
//   Virtual abstract base class for templated reactingMultiphaseCloud
// SourceFiles
//   reacting_multiphase_cloud.cpp
#ifndef reacting_multiphase_cloud_hpp_
#define reacting_multiphase_cloud_hpp_
#include "type_info.hpp"
namespace mousse
{
class reactingMultiphaseCloud
{
public:
  //- Runtime type information
  TYPE_NAME("reactingMultiphaseCloud");
  // Constructors
    //- Null constructor
    reactingMultiphaseCloud();
    //- Disallow default bitwise copy construct
    reactingMultiphaseCloud(const reactingMultiphaseCloud&) = delete;
    //- Disallow default bitwise assignment
    reactingMultiphaseCloud& operator=(const reactingMultiphaseCloud&) = delete;
  //- Destructor
  virtual ~reactingMultiphaseCloud();
};
}  // namespace mousse
#endif
