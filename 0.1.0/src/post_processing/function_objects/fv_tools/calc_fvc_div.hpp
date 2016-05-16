#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_DIV_CALC_FVC_DIV_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_DIV_CALC_FVC_DIV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcFvcDiv
// Group
//   grpFVFunctionObjects
// Description
//   This function object calculates the divergence of a field.  The operation is
//   limited to surfaceScalarFields and volumeVector fields, and the output is a
//   volume scalar field.

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "point_field_fwd.hpp"
#include "ofstream.hpp"
#include "switch.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class dimensionSet;


class calcFvcDiv
{
  // Private data
    //- Name of this calcFvcDiv object
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
    //- Helper function to create/store/return the divergence field
    volScalarField& divField
    (
      const word& gradName,
      const dimensionSet& dims
    );
    //- Helper function to calculate the divergence of different field types
    template<class FieldType>
    void calcDiv
    (
      const word& fieldName,
      const word& resultName,
      bool& processed
    );
public:
  //- Runtime type information
  TYPE_NAME("calcFvcDiv");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    calcFvcDiv
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    calcFvcDiv(const calcFvcDiv&) = delete;
    //- Disallow default bitwise assignment
    calcFvcDiv& operator=(const calcFvcDiv&) = delete;
  //- Destructor
  virtual ~calcFvcDiv();
  // Member Functions
    //- Return name of the set of calcFvcDiv
    virtual const word& name() const { return name_; }
    //- Read the calcFvcDiv data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the calcFvcDiv and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};

}  // namespace mousse

#include "calc_fvc_div.ipp"

#endif
