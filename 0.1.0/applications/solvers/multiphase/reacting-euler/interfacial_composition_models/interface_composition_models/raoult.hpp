#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_RAOULT_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_RAOULT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Raoult
// Description
//   Raoult's law of ideal mixing. A separate composition model is given for
//   each species. The composition of a species is equal to the value given by
//   the model scaled by the species fraction in the bulk of the other phase.
// SourceFiles
//   raoult.cpp
#include "_interface_composition_model.hpp"
namespace mousse
{
class phasePair;
namespace interfaceCompositionModels
{
template<class Thermo, class OtherThermo>
class Raoult
:
  public InterfaceCompositionModel<Thermo, OtherThermo>
{
private:
  // Private data
    //- Non-vapour species fraction
    volScalarField YNonVapour_;
    //- Non-vapour species fraction derivative w.r.t. temperature
    volScalarField YNonVapourPrime_;
    //- Species' individual composition models
    HashTable<autoPtr<interfaceCompositionModel> > speciesModels_;
public:
  //- Runtime type information
  TYPE_NAME("Raoult");
  // Constructors
    //- Construct from components
    Raoult
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Raoult();
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
#   include "raoult.cpp"
#endif
#endif
