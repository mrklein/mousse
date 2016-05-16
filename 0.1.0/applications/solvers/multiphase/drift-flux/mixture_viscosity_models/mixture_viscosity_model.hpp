#ifndef MIXTURE_VISCOSITY_MODEL_HPP_
#define MIXTURE_VISCOSITY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixtureViscosityModel
// Description
//   An abstract base class for incompressible mixtureViscosityModels.

#include "dictionary.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_scalar.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class mixtureViscosityModel
{
protected:
  // Protected data
    word name_;
    dictionary viscosityProperties_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
public:
  //- Runtime type information
  TYPE_NAME("mixtureViscosityModel");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      mixtureViscosityModel,
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
    static autoPtr<mixtureViscosityModel> New
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  // Constructors
    //- Construct from components
    mixtureViscosityModel
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
    //- Disallow copy construct
    mixtureViscosityModel(const mixtureViscosityModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const mixtureViscosityModel&) = delete;
  //- Destructor
  virtual ~mixtureViscosityModel()
  {}
  // Member Functions
    //- Return the phase transport properties dictionary
    const dictionary& viscosityProperties() const
    {
      return viscosityProperties_;
    }
    //- Return the mixture viscosity
    //  given the viscosity of the continuous phase
    virtual tmp<volScalarField> mu(const volScalarField& muc) const = 0;
    //- Read transportProperties dictionary
    virtual bool read(const dictionary& viscosityProperties) = 0;
};

}  // namespace mousse

#endif

