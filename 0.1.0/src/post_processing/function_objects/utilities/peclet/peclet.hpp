#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PECLET_PECLET_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PECLET_PECLET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Peclet
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates and outputs the Peclet number as a
//   surfaceScalarField.
// SourceFiles
//   peclet.cpp
//   io_peclet.hpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "ofstream.hpp"
#include "switch.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class Peclet
{
  // Private data
    //- Name of this set of Peclet objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of flux field, default is "phi"
    word phiName_;
    //- Name of density field (compressible cases only), default is "rho"
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("Peclet");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    Peclet
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    Peclet(const Peclet&) = delete;
    //- Disallow default bitwise assignment
    Peclet& operator=(const Peclet&) = delete;
  //- Destructor
  virtual ~Peclet();
  // Member Functions
    //- Return name of the set of Peclet
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the Peclet data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the Peclet and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#endif
