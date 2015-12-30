// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DSMCBaseCloud
// Description
//   Virtual abstract base class for templated DSMCCloud
// SourceFiles
//   dsmc_base_cloud.cpp
#ifndef dsmc_base_cloud_hpp_
#define dsmc_base_cloud_hpp_
#include "vol_fields.hpp"
namespace mousse
{
class DSMCBaseCloud
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    DSMCBaseCloud(const DSMCBaseCloud&);
    //- Disallow default bitwise assignment
    void operator=(const DSMCBaseCloud&);
public:
  //- Runtime type information
  TypeName("DSMCBaseCloud");
  // Constructors
    //- Null constructor
    DSMCBaseCloud();
  //- Destructor
  virtual ~DSMCBaseCloud();
};
}  // namespace mousse
#endif
