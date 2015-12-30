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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    reactingMultiphaseCloud(const reactingMultiphaseCloud&);
    //- Disallow default bitwise assignment
    void operator=(const reactingMultiphaseCloud&);
public:
  //- Runtime type information
  TypeName("reactingMultiphaseCloud");
  // Constructors
    //- Null constructor
    reactingMultiphaseCloud();
  //- Destructor
  virtual ~reactingMultiphaseCloud();
};
}  // namespace mousse
#endif
