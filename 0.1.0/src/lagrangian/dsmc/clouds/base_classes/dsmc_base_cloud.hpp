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
public:
  //- Runtime type information
  TYPE_NAME("DSMCBaseCloud");
  // Constructors
    //- Null constructor
    DSMCBaseCloud();
    //- Disallow default bitwise copy construct
    DSMCBaseCloud(const DSMCBaseCloud&) = delete;
    //- Disallow default bitwise assignment
    DSMCBaseCloud& operator=(const DSMCBaseCloud&) = delete;
  //- Destructor
  virtual ~DSMCBaseCloud();
};
}  // namespace mousse
#endif
