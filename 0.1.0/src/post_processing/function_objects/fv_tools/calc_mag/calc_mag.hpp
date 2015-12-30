// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcMag
// Group
//   grpFVFunctionObjects
// Description
//   This function object calculates the magnitude of a field.  The operation
//   can be applied to any volume or surface fieldsm and the output is a
//   volume or surface scalar field.
// SourceFiles
//   calc_mag.cpp
//   i_ocalc_mag.hpp
#ifndef calc_mag_hpp_
#define calc_mag_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "point_field_fwd.hpp"
#include "ofstream.hpp"
#include "switch.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class dimensionSet;
class calcMag
{
  // Private data
    //- Name of this calcMag object
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of field to process
    word fieldName_;
    //- Name of result field
    word resultName_;
  // Private Member Functions
    //- Helper function to create/store/return the mag field
    template<class FieldType>
    FieldType& magField(const word& magName, const dimensionSet& dims);
    //- Helper function to calculate the magnitude of different field types
    template<class Type>
    void calc
    (
      const word& fieldName,
      const word& resultName,
      bool& processed
    );
    //- Disallow default bitwise copy construct
    calcMag(const calcMag&);
    //- Disallow default bitwise assignment
    void operator=(const calcMag&);
public:
  //- Runtime type information
  TypeName("calcMag");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    calcMag
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~calcMag();
  // Member Functions
    //- Return name of the set of calcMag
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the calcMag data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the calcMag and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#ifdef NoRepository
  #include "calc_mag_templates.cpp"
#endif
#endif
