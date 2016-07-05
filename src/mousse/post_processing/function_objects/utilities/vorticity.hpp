#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_VORTICITY_VORTICITY_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_VORTICITY_VORTICITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vorticity
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates the vorticity, the curl of the velocity.

#include "vol_fields_fwd.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;


class vorticity
{
  // Private data
    //- Name of this set of vorticity objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of velocity field, default is "U"
    word UName_;
    //- Name of vorticity field
    word outputName_;
public:
  //- Runtime type information
  TYPE_NAME("vorticity");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    vorticity
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    vorticity(const vorticity&) = delete;
    //- Disallow default bitwise assignment
    vorticity& operator=(const vorticity&) = delete;
  //- Destructor
  virtual ~vorticity();
  // Member Functions
    //- Return name of the set of vorticity
    virtual const word& name() const { return name_; }
    //- Read the vorticity data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the vorticity and write
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

