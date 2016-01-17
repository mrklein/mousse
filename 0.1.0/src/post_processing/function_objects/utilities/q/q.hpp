// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Q
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates and outputs the second invariant of the
//   velocity gradient tensor [1/s^2].
//   \f[
//     Q = 0.5(sqr(tr(\nabla U)) - tr(((\nabla U) \cdot (\nabla U))))
//   \f]
// SourceFiles
//   q.cpp
//   ioq.hpp
#ifndef q_hpp_
#define q_hpp_
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
class Q
{
  // Private data
    //- Name of this set of Q objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of velocity field, default is "U"
    word UName_;
public:
  //- Runtime type information
  TYPE_NAME("Q");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    Q
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    Q(const Q&) = delete;
    //- Disallow default bitwise assignment
    Q& operator=(const Q&) = delete;
  //- Destructor
  virtual ~Q();
  // Member Functions
    //- Return name of the set of Q
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the Q data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the Q and write
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
