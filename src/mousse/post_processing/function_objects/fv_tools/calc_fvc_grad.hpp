#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_GRAD_CALC_FVC_GRAD_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_GRAD_CALC_FVC_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcFvcGrad
// Group
//   grpFVFunctionObjects
// Description
//   This function object calculates the gradient of a field.  The operation is
//   limited to scalar and vector volume or surface fields, and the output is a
//   volume vector or tensor field.

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


class calcFvcGrad
{
  // Private data
    //- Name of this calcFvcGrad object
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
    //- Helper function to create/store/return the gradient field
    template<class Type>
    GeometricField
    <
      typename outerProduct<vector, Type>::type,
      fvPatchField,
      volMesh
    >&
    gradField(const word& gradName, const dimensionSet& dims);
    //- Helper function to calculate the gradient of different field types
    template<class Type>
    void calcGrad
    (
      const word& fieldName,
      const word& resultName,
      bool& processed
    );
public:
  //- Runtime type information
  TYPE_NAME("calcFvcGrad");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    calcFvcGrad
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    calcFvcGrad(const calcFvcGrad&) = delete;
    //- Disallow default bitwise assignment
    calcFvcGrad& operator=(const calcFvcGrad&) = delete;
  //- Destructor
  virtual ~calcFvcGrad();
  // Member Functions
    //- Return name of the set of calcFvcGrad
    virtual const word& name() const { return name_; }
    //- Read the calcFvcGrad data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the calcFvcGrad and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};

}  // namespace mousse

#include "calc_fvc_grad.ipp"

#endif

