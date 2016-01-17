// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulenceFields
// Group
//   grpFieldFunctionObjects
// Description
//   This function object stores turbulence fields on the mesh database for
//   further manipulation.
//   Fields are stored as copies of the original, with the prefix
//   "tubulenceModel:", e.g.
//     turbulenceModel:R
//   Example of function object specification:
//   \verbatim
//   turbulenceFields1
//   {
//     type        turbulenceFields;
//     functionObjectLibs ("libutilityFunctionObjects.so");
//     ...
//     fields
//     (
//       R
//       devRhoReff
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description                 | Required | Default value
//     type         | type name: processorField   | yes      |
//     fields       | fields to store (see below) | yes      |
//   \endtable
//   Where \c fields can include:
//   \plaintable
//     k           | turbulence kinetic energy
//     epsilon     | turbulence kinetic energy dissipation rate
//     nut         | turbulence viscosity (incompressible)
//     nuEff       | effective turbulence viscosity (incompressible)
//     mut         | turbulence viscosity (compressible)
//     muEff       | effective turbulence viscosity (compressible)
//     alphat      | turbulence thermal diffusivity (compressible)
//     alphaEff    | effective turbulence thermal diffusivity (compressible)
//     R           | Reynolds stress tensor
//     devReff     | Deviatoric part of the effective Reynolds stress
//     devRhoReff  | Divergence of the Reynolds stress
//   \endplaintable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   turbulence_fields.cpp
#ifndef turbulence_fields_hpp_
#define turbulence_fields_hpp_
#include "hash_set.hpp"
#include "ioobject.hpp"
#include "named_enum.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class turbulenceFields
{
public:
  enum compressibleField
  {
    cfK,
    cfEpsilon,
    cfMut,
    cfMuEff,
    cfAlphat,
    cfAlphaEff,
    cfR,
    cfDevRhoReff
  };
  static const NamedEnum<compressibleField, 8> compressibleFieldNames_;
  enum incompressibleField
  {
    ifK,
    ifEpsilon,
    ifNut,
    ifNuEff,
    ifR,
    ifDevReff
  };
  static const NamedEnum<incompressibleField, 6> incompressibleFieldNames_;
  static const word modelName;
protected:
  // Protected data
    //- Name of this set of turbulenceFields object
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Fields to load
    wordHashSet fieldSet_;
  // Protected Member Functions
    //- Return true if compressible turbulence model is identified
    bool compressible();
    //- Process the turbulence field
    template<class Type>
    void processField
    (
      const word& fieldName,
      const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvalue
    );
public:
  //- Runtime type information
  TYPE_NAME("turbulenceFields");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    turbulenceFields
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    turbulenceFields(const turbulenceFields&) = delete;
    //- Disallow default bitwise assignment
    turbulenceFields& operator=(const turbulenceFields&) = delete;
  //- Destructor
  virtual ~turbulenceFields();
  // Member Functions
    //- Return name of the turbulenceFields object
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the field min/max data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write
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
  #include "turbulence_fields_templates.cpp"
#endif
#endif
