#ifndef THERMOPHYSICAL_MODELS_SOLID_THERMO_HE_SOLID_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_THERMO_HE_SOLID_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heSolidThermo
// Description
//   Energy for a solid mixture

#include "he_thermo.hpp"


namespace mousse {

template<class BasicSolidThermo, class MixtureType>
class heSolidThermo
:
  public heThermo<BasicSolidThermo, MixtureType>
{
  // Private Member Functions
    //- Calculate the thermo variables
    void calculate();
public:
  //- Runtime type information
  TYPE_NAME("heSolidThermo");
  // Constructors
    //- Construct from mesh and phase name
    heSolidThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh, dictionary and phase name
    heSolidThermo
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName
    );
    //- Disable construct as copy
    heSolidThermo(const heSolidThermo<BasicSolidThermo, MixtureType>&) = delete;
  //- Destructor
  virtual ~heSolidThermo();
  // Member functions
    //- Update properties
    virtual void correct();
    // Derived thermal properties
      //- Anisotropic thermal conductivity [W/m/K]
      virtual tmp<volVectorField> Kappa() const;
      //- Return true if thermal conductivity is isotropic
      virtual bool isotropic() const
      {
        return MixtureType::thermoType::isotropic;
      }
    // Per patch calculation
      //- Anisotropic thermal conductivity [W/m/K]
      virtual tmp<vectorField> Kappa(const label patchI) const;
};

}  // namespace mousse

#include "he_solid_thermo.ipp"

#endif
