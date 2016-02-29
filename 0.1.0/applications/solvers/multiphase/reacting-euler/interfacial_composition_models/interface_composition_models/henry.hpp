#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_HENRY_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_HENRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Henry
// Description
//   Henry's law for gas solubiliy in liquid. The concentration of the dissolved
//   species in the liquid is proportional to its partial pressure in the gas.
//   The dimensionless constant of proportionality between concentrations on
//   each side of the interface is \f$k\f$, and is given for each species.
//   Mixing in the gas is assumed to be ideal.
// SourceFiles
//   henry.cpp
#include "_interface_composition_model.hpp"
namespace mousse
{
class phasePair;
namespace interfaceCompositionModels
{
template<class Thermo, class OtherThermo>
class Henry
:
  public InterfaceCompositionModel<Thermo, OtherThermo>
{
private:
  // Private data
    //- Solubility coefficients
    const scalarList k_;
    //- The remaining solvent species fraction
    volScalarField YSolvent_;
public:
  //- Runtime type information
  TYPE_NAME("Henry");
  // Constructors
    //- Construct from components
    Henry
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Henry();
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
#   include "henry.cpp"
#endif
#endif
