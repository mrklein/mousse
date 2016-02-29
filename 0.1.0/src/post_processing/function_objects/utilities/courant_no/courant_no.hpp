#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_COURANT_NO_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_COURANT_NO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CourantNo
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates and outputs the Courant number as a
//   volScalarField.  The field is stored on the mesh database so that it can
//   be retrieved and used for other applications.
// SourceFiles
//   courant_no.cpp
//   io_courant_no.hpp
#include "vol_fields.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class CourantNo
{
  // Private data
    //- Name of this set of CourantNo objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of flux field, default is "phi"
    word phiName_;
    //- Name of density field (optional)
    word rhoName_;
  // Private Member Functions
    //- Divide Co by rho if required
    tmp<volScalarField::DimensionedInternalField> byRho
    (
      const tmp<volScalarField::DimensionedInternalField>& Co
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("CourantNo");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    CourantNo
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    CourantNo(const CourantNo&) = delete;
    //- Disallow default bitwise assignment
    CourantNo& operator=(const CourantNo&) = delete;
  //- Destructor
  virtual ~CourantNo();
  // Member Functions
    //- Return name of the set of CourantNo
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the CourantNo data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the CourantNo and write
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
