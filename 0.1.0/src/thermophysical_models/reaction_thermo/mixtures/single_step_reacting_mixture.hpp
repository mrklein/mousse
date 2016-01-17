// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::singleStepReactingMixture
// Description
//   Single step reacting mixture
// SourceFiles
//   single_step_reacting_mixture.cpp
#ifndef single_step_reacting_mixture_hpp_
#define single_step_reacting_mixture_hpp_
#include "chemistry_reader.hpp"
#include "reacting_mixture.hpp"
namespace mousse
{
template<class ThermoType>
class singleStepReactingMixture
:
  public reactingMixture<ThermoType>
{
protected:
  // Protected data
    //- Stoichiometric air-fuel mass ratio
    dimensionedScalar stoicRatio_;
    //- Stoichiometric oxygen-fuel mass ratio
    dimensionedScalar s_;
    //- Heat of combustion [J/Kg]
    dimensionedScalar qFuel_;
    //- Stoichiometric coefficient for the reaction.
    scalarList specieStoichCoeffs_;
    //- Mass concentrations at stoichiometric mixture for fres.
    scalarList Yprod0_;
    //- List of components residual
    PtrList<volScalarField> fres_;
    //- Inert specie index
    label inertIndex_;
    //- Fuel specie index
    label fuelIndex_;
    //- List to indicate if specie is produced/consumed
    List<int> specieProd_;
  // Protected member functions
    //- Calculate qFuel
    void calculateqFuel();
    //- Calculate air/fuel and oxygen/fuel ratio
    void massAndAirStoichRatios();
    //- Calculate maximum products at stoichiometric mixture
    void calculateMaxProducts();
public:
  //- The type of thermo package this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    singleStepReactingMixture
    (
      const dictionary&,
      const fvMesh&,
      const word&
    );
    //- Disallow default bitwise copy construct
    singleStepReactingMixture(const singleStepReactingMixture&) = delete;
    //- Disallow default bitwise assignment
    singleStepReactingMixture& operator=
    (
      const singleStepReactingMixture&
    ) = delete;
  //- Destructor
  virtual ~singleStepReactingMixture()
  {}
  // Member functions
    //- Calculates the residual for all components
    void fresCorrect();
    // Access functions
      //- Return the stoichiometric air-fuel mass ratio
      inline const dimensionedScalar stoicRatio() const;
      //- Return the Stoichiometric oxygen-fuel mass ratio
      inline const dimensionedScalar s() const;
      //- Return the heat of combustion [J/Kg]
      inline const dimensionedScalar qFuel() const;
      //- Return the stoichiometric coefficient for the reaction
      inline const List<scalar>& specieStoichCoeffs() const;
      //- Return the list of components residual
      inline tmp<volScalarField> fres(const label index) const;
      //- Return the inert specie index
      inline label inertIndex() const;
      //- Return the fuel specie index
      inline label fuelIndex() const;
      //- Return the list to indicate if specie is produced/consumed
      inline const List<int>& specieProd() const;
      //- Return the list of products mass concentrations
      inline const scalarList& Yprod0() const;
    // I-O
      //- Read dictionary
      void read(const dictionary&);
};
}  // namespace mousse
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::stoicRatio() const
{
  return stoicRatio_;
}
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::s() const
{
  return s_;
}
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::qFuel() const
{
  return qFuel_;
}
template<class ThermoType>
inline const mousse::List<mousse::scalar>&
mousse::singleStepReactingMixture<ThermoType>::specieStoichCoeffs() const
{
  return specieStoichCoeffs_;
}
template<class ThermoType>
inline mousse::tmp<mousse::volScalarField>
mousse::singleStepReactingMixture<ThermoType>::fres
(
  const label index
) const
{
  return fres_[index];
}
template<class ThermoType>
inline mousse::label
mousse::singleStepReactingMixture<ThermoType>::inertIndex() const
{
  return inertIndex_;
}
template<class ThermoType>
inline mousse::label
mousse::singleStepReactingMixture<ThermoType>::fuelIndex() const
{
  return fuelIndex_;
}
template<class ThermoType>
inline const mousse::List<int>&
mousse::singleStepReactingMixture<ThermoType>::specieProd() const
{
  return specieProd_;
}
template<class ThermoType>
inline const mousse::scalarList&
mousse::singleStepReactingMixture<ThermoType>::Yprod0() const
{
  return Yprod0_;
}
#ifdef NoRepository
#   include "single_step_reacting_mixture.cpp"
#endif
#endif
