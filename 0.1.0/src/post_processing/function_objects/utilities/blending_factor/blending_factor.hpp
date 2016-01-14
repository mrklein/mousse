// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blendingFactor
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object calculates and outputs the blendingFactor as used by
//   the bended convection schemes.  The output is a volume field (cells) whose
//   value is calculated via the maximum blending factor for any cell face.
// SourceFiles
//   blending_factor.cpp
//   i_oblending_factor.hpp
#ifndef blending_factor_hpp_
#define blending_factor_hpp_
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
class blendingFactor
{
  // Private data
    //- Name of this set of blendingFactor objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of flux field, default is "phi"
    word phiName_;
    //- Field name
    word fieldName_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    blendingFactor(const blendingFactor&);
    //- Disallow default bitwise assignment
    void operator=(const blendingFactor&);
    //- Return the blending factor field from the database
    template<class Type>
    volScalarField& factor
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    );
    //- Calculate the blending factor
    template<class Type>
    void calc();
public:
  //- Runtime type information
  TYPE_NAME("blendingFactor");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    blendingFactor
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~blendingFactor();
  // Member Functions
    //- Return name of the set of blendingFactor
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the blendingFactor data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the blendingFactor and write
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
  #include "blending_factor_templates.cpp"
#endif
#endif
