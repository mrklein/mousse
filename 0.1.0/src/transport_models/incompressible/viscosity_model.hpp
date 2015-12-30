// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::viscosityModel
// Description
//   An abstract base class for incompressible viscosityModels.
//   The strain rate is defined by:
//     mag(symm(grad(U)))
// SourceFiles
//   viscosity_model.cpp
//   viscosity_model_new.cpp
#ifndef viscosity_model_hpp_
#define viscosity_model_hpp_
#include "dictionary.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_scalar.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class viscosityModel
{
protected:
  // Protected data
    word name_;
    dictionary viscosityProperties_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
  // Private Member Functions
    //- Disallow copy construct
    viscosityModel(const viscosityModel&);
    //- Disallow default bitwise assignment
    void operator=(const viscosityModel&);
public:
  //- Runtime type information
  TypeName("viscosityModel");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      viscosityModel,
      dictionary,
      (
        const word& name,
        const dictionary& viscosityProperties,
        const volVectorField& U,
        const surfaceScalarField& phi
      ),
      (name, viscosityProperties, U, phi)
    );
  // Selectors
    //- Return a reference to the selected viscosity model
    static autoPtr<viscosityModel> New
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  // Constructors
    //- Construct from components
    viscosityModel
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~viscosityModel()
  {}
  // Member Functions
    //- Return the phase transport properties dictionary
    const dictionary& viscosityProperties() const
    {
      return viscosityProperties_;
    }
    //- Return the strain rate
    tmp<volScalarField> strainRate() const;
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const = 0;
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const = 0;
    //- Correct the laminar viscosity
    virtual void correct() = 0;
    //- Read transportProperties dictionary
    virtual bool read(const dictionary& viscosityProperties) = 0;
};
}  // namespace mousse
#endif
