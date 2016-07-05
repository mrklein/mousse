#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_MIXTURE_PROPERTIES_LIQUID_MIXTURE_PROPERTIES_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_MIXTURE_PROPERTIES_LIQUID_MIXTURE_PROPERTIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liquidMixtureProperties
// Description
//   A mixture of liquids
//   An example of a two component liquid mixture:
//   \verbatim
//     <parentDictionary>
//     {
//       H2O
//       {
//         defaultCoeffs   yes;     // employ default coefficients
//       }
//       C7H16
//       {
//         defaultCoeffs   no;
//         C7H16Coeffs
//         {
//           ... user defined properties for C7H16
//         }
//       }
//     }
//   \endverbatim
// SourceFiles
//   liquid_mixture_properties.cpp
//   see_also
//   foam::liquid_properties
#include "word.hpp"
#include "scalar_field.hpp"
#include "ptr_list.hpp"
#include "liquid_properties.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class liquidMixtureProperties
{
  // Private data
    //- Maximum reduced temperature
    static const scalar TrMax;
    //- The names of the liquids
    List<word> components_;
    //- The liquid properties
    PtrList<liquidProperties> properties_;
public:
  // Constructors
    //- Construct from dictionary
    liquidMixtureProperties(const dictionary& dict);
    //- Construct copy
    liquidMixtureProperties(const liquidMixtureProperties& lm);
    //- Construct and return a clone
    virtual autoPtr<liquidMixtureProperties> clone() const
    {
      return autoPtr<liquidMixtureProperties>
      (
        new liquidMixtureProperties(*this)
      );
    }
  //- Destructor
  virtual ~liquidMixtureProperties()
  {}
  // Selectors
    //- Select construct from dictionary
    static autoPtr<liquidMixtureProperties> New(const dictionary&);
  // Member Functions
    //- Return the liquid names
    inline const List<word>& components() const
    {
      return components_;
    }
    //- Return the liquid properties
    inline const PtrList<liquidProperties>& properties() const
    {
      return properties_;
    }
    //- Return the number of liquids in the mixture
    inline label size() const
    {
      return components_.size();
    }
    //- Calculate the critical temperature of mixture
    scalar Tc(const scalarField& X) const;
    //- Invert the vapour pressure relationship to retrieve the boiling
    //  temperature of the mixture as a function of pressure
    scalar pvInvert(const scalar p, const scalarField& X) const;
    //- Return pseudocritical temperature according to Kay's rule
    scalar Tpc(const scalarField& X) const;
    //- Return pseudocritical pressure (modified Prausnitz and Gunn)
    scalar Ppc(const scalarField& X) const;
    //- Return pseudo triple point temperature (mole averaged formulation)
    scalar Tpt(const scalarField& X) const;
    //- Return mixture accentric factor
    scalar omega(const scalarField& X) const;
    //- Return the surface molar fractions
    scalarField Xs
    (
      const scalar p,
      const scalar Tg,
      const scalar Tl,
      const scalarField& Xg,
      const scalarField& Xl
    ) const;
    //- Calculate the mean molecular weight [kg/kmol]
    //  from mole fractions
    scalar W(const scalarField& X) const;
    //- Returns the mass fractions corresponding to the given mole fractions
    scalarField Y(const scalarField& X) const;
    //- Returns the mole fractions corresponding to the given mass fractions
    scalarField X(const scalarField& Y) const;
    //- Calculate the mixture density [kg/m^3]
    scalar rho
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Calculate the mixture vapour pressure [Pa]
    scalar pv
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Calculate the mixture latent heat [J/kg]
    scalar hl
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Calculate the mixture heat capacity [J/(kg K)]
    scalar Cp
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Estimate mixture surface tension [N/m]
    scalar sigma
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Calculate the mixture viscosity [Pa s]
    scalar mu
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Estimate thermal conductivity  [W/(m K)]
    //  Li's method, Eq. 10-12.27 - 10.12-19
    scalar K
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
    //- Vapour diffussivity [m2/s]
    scalar D
    (
      const scalar p,
      const scalar T,
      const scalarField& X
    ) const;
};
}  // namespace mousse
#endif
