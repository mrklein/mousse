#ifndef REGION_MODELS_THERMAL_BAFFLE_MODELS_NO_THERMO_HPP_
#define REGION_MODELS_THERMAL_BAFFLE_MODELS_NO_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noThermo
// Description
//   Dummy surface pyrolysis model for 'none'

#include "thermal_baffle_model.hpp"
#include "vol_fields_fwd.hpp"


namespace mousse {
namespace regionModels {
namespace thermalBaffleModels {

class noThermo
:
  public thermalBaffleModel
{
protected:
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from type name and mesh
    noThermo(const word& modelType, const fvMesh& mesh);
    //- Construct from components and dict
    noThermo
    (
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    noThermo(const noThermo&) = delete;
    //- Disallow default bitwise assignment
    noThermo& operator=(const noThermo&) = delete;
  //- Destructor
  virtual ~noThermo();
  // Member Functions
    // Thermo properties
      //- Return const reference to the solidThermo
      virtual const solidThermo& thermo() const;
    // Fields
      //- Return the film specific heat capacity [J/kg/K]
      virtual const tmp<volScalarField> Cp() const;
      //- Return solid absortivity [1/m]
      virtual const volScalarField& kappaRad() const;
      //- Return the film mean temperature [K]
      virtual const volScalarField& T() const;
      //- Return density [Kg/m3]
      virtual const volScalarField& rho() const;
      //- Return thermal conductivity [W/m/K]
      virtual const volScalarField& kappa() const;
    // Evolution
      //- Pre-evolve film
      virtual void preEvolveRegion();
      //- Evolve the film equations
      virtual void evolveRegion();
};

}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse

#endif

