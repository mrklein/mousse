// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cloud
// Description
//   A cloud is a collection of lagrangian particles
// SourceFiles
//   cloud.cpp

#ifndef cloud_hpp_
#define cloud_hpp_

#include "object_registry.hpp"

namespace mousse
{

// Forward declaration of classes
class mapPolyMesh;

class cloud
:
  public objectRegistry
{
public:

  //- Runtime type information
  TYPE_NAME("cloud");

    //- The prefix to local: %lagrangian
    static const word prefix;

    //- The default cloud name: %defaultCloud
    static word defaultName;

  // Constructors

    //- Construct for the given objectRegistry and named cloud instance
    cloud(const objectRegistry&, const word& cloudName = "");

    //- Disallow default bitwise copy construct
    cloud(const cloud&) = delete;

    //- Disallow default bitwise assignment
    void operator=(const cloud&) = delete;

  //- Destructor
  virtual ~cloud();

  // Member Functions

    // Edit

      //- Remap the cells of particles corresponding to the
      //  mesh topology change
      virtual void autoMap(const mapPolyMesh&);

};

}  // namespace mousse

#endif
