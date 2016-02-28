// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Saturated
// Description
//   Model which uses a saturation pressure model for a single species to
//   calculate the interface composition.
// SourceFiles
//   saturated.cpp
#ifndef saturated_hpp_
#define saturated_hpp_
#include "_interface_composition_model.hpp"
#include "saturation_model.hpp"
namespace mousse
{
class phasePair;
namespace interfaceCompositionModels
{
template<class Thermo, class OtherThermo>
class Saturated
:
  public InterfaceCompositionModel<Thermo, OtherThermo>
{
protected:
  // Private data
    //- Saturated species name
    word saturatedName_;
    //- Saturated species index
    label saturatedIndex_;
    //- Saturation pressure model
    autoPtr<saturationModel> saturationModel_;
  // Private Member Functions
    //- Constant of propotionality between partial pressure and mass
    //  fraction
    tmp<volScalarField> wRatioByP() const;
public:
  //- Runtime type information
  TYPE_NAME("Saturated");
  // Constructors
    //- Construct from components
    Saturated
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Saturated();
  // Member Functions
    //- Update the composition
    virtual void update(const volScalarField& Tf);
    //- The interface species fraction
    virtual tmp<volScalarField> Yf
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
    //- The interface species fraction derivative w.r.t. temperature
    virtual tmp<volScalarField> YfPrime
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
};
}  // namespace interfaceCompositionModels
}  // namespace mousse
#ifdef NoRepository
#   include "saturated.cpp"
#endif
#endif
