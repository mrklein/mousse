#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_REACTING_MULTIPHASE_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_REACTING_MULTIPHASE_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMultiphaseCloud
// Description
//   Virtual abstract base class for templated reactingMultiphaseCloud

#include "type_info.hpp"


namespace mousse {

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

