#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_LAMBDA2_LAMBDA2_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_LAMBDA2_LAMBDA2_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Lambda2
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates and outputs the second largest eigenvalue
//   of the sum of the square of the symmetrical and anti-symmetrical parts of
//   the velocity gradient tensor.

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "ofstream.hpp"
#include "switch.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;


class Lambda2
{
  // Private data
    //- Name of this set of Lambda2 objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of velocity field, default is "U"
    word UName_;
public:
  //- Runtime type information
  TYPE_NAME("Lambda2");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    Lambda2
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    Lambda2(const Lambda2&) = delete;
    //- Disallow default bitwise assignment
    Lambda2& operator=(const Lambda2&) = delete;
  //- Destructor
  virtual ~Lambda2();
  // Member Functions
    //- Return name of the set of Lambda2
    virtual const word& name() const { return name_; }
    //- Read the Lambda2 data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the Lambda2 and write
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

