// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sprayCloud
// Description
//   Virtual abstract base class for templated SprayCloud
// SourceFiles
//   spray_cloud.cpp
#ifndef spray_cloud_hpp_
#define spray_cloud_hpp_
#include "type_info.hpp"
namespace mousse
{
class sprayCloud
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    sprayCloud(const sprayCloud&);
    //- Disallow default bitwise assignment
    void operator=(const sprayCloud&);
public:
  //- Runtime type information
  TypeName("sprayCloud");
  // Constructors
    //- Null constructor
    sprayCloud();
  //- Destructor
  virtual ~sprayCloud();
};
}  // namespace mousse
#endif
