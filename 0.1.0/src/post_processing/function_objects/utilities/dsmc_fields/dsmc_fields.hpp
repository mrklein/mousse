// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dsmcFields
// Group
//   grpUtilitiesFunctionObjects
// Description
//   Calculate intensive fields:
//   - UMean
//   - translationalT
//   - internalT
//   - overallT
//   from averaged extensive fields from a DSMC calculation.
// SourceFiles
//   dsmc_fields.cpp
//   i_odsmc_fields.hpp
#ifndef dsmc_fields_hpp_
#define dsmc_fields_hpp_
#include "type_info.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class dsmcFields
{
  // Private data
    //- Name of this set of dsmcFields objects
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    dsmcFields(const dsmcFields&);
    //- Disallow default bitwise assignment
    void operator=(const dsmcFields&);
public:
  //- Runtime type information
  TypeName("dsmcFields");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    dsmcFields
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~dsmcFields();
  // Member Functions
    //- Return name of the set of dsmcFields
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the dsmcFields data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the dsmcFields and write
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
