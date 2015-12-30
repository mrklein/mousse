// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingCloud
// Description
//   Virtual abstract base class for templated ReactingCloud
// SourceFiles
//   reacting_cloud.cpp
#ifndef reacting_cloud_hpp_
#define reacting_cloud_hpp_
#include "type_info.hpp"
namespace mousse
{
class reactingCloud
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    reactingCloud(const reactingCloud&);
    //- Disallow default bitwise assignment
    void operator=(const reactingCloud&);
public:
  //- Runtime type information
  TypeName("reactingCloud");
  // Constructors
    //- Null constructor
    reactingCloud();
  //- Destructor
  virtual ~reactingCloud();
};
}  // namespace mousse
#endif
