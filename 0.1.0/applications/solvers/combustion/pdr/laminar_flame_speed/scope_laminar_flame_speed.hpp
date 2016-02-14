// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeedModels::SCOPE
// Description
//   Laminar flame speed obtained from the SCOPE correlation.
//   Seven parameters are specified in terms of polynomial functions of
//   stoichiometry. Two polynomials are fitted, covering different parts of the
//   flammable range. If the mixture is outside the fitted range, linear
//   interpolation is used between the extreme of the polynomio and the upper or
//   lower flammable limit with the Markstein number constant.
//   Variations of pressure and temperature from the reference values are taken
//   into account through \f$ pexp \f$ and \f$ texp \f$
//   The laminar burning velocity fitting polynomial is:
//   \f$ Su = a_{0}(1+a_{1}x+K+..a_{i}x^{i}..+a_{6}x^{6}) (p/p_{ref})^{pexp}
//   (T/T_{ref})^{texp} \f$
//   where:
//     \f$ a_{i} \f$ are the polinomial coefficients.
//     \f$ pexp \f$ and \f$ texp \f$ are the pressure and temperature factors
//     respectively.
//     \f$ x \f$ is the equivalence ratio.
//     \f$ T_{ref} \f$ and \f$ p_{ref} \f$ are the temperature and pressure
//     references for the laminar burning velocity.
// SourceFiles
//   scope_laminar_flame_speed.cpp
#ifndef SCOPE_HPP_
#define SCOPE_HPP_
#include "laminar_flame_speed.hpp"
namespace mousse
{
namespace laminarFlameSpeedModels
{
class SCOPE
:
  public laminarFlameSpeed
{
  // Private Data
    class polynomial
    :
      public FixedList<scalar, 7>
    {
    public:
      //- Lower limit
      scalar ll;
      //- Upper polynomial limit
      scalar ul;
      //- Value at lower limit
      scalar llv;
      //- Value at upper limit
      scalar ulv;
      //- Changeover point from lower to upper polynomial
      scalar lu;
      //- Construct from dictionary
      polynomial(const dictionary& polyDict);
    };
    dictionary coeffsDict_;
    //- Lower flamability limit
    scalar LFL_;
    //- Upper flamability limit
    scalar UFL_;
    //- Lower Su polynomial
    polynomial SuPolyL_;
    //- Upper Su polynomial
    polynomial SuPolyU_;
    //- Temperature correction exponent
    scalar Texp_;
    //- Pressure correction exponent
    scalar pexp_;
    //- Lower Ma polynomial
    polynomial MaPolyL_;
    //- Upper Ma polynomial
    polynomial MaPolyU_;
  // Private member functions
    //- Polynomial evaluated from the given equivalence ratio
    //  and polynomial coefficients
    static inline scalar polyPhi(scalar phi, const polynomial& a);
    //- Laminar flame speed evaluated from the given equivalence ratio
    //  at the reference temperature and pressure
    inline scalar SuRef(scalar phi) const;
    //- Markstein evaluated from the given equivalence ratio
    inline scalar Ma(scalar phi) const;
    //- Laminar flame speed evaluated from the given equivalence ratio
    //  corrected for temperature and pressure dependence
    inline scalar Su0pTphi(scalar p, scalar Tu, scalar phi) const;
    //- Laminar flame speed evaluated from the given uniform
    //  equivalence ratio corrected for temperature and pressure dependence
    tmp<volScalarField> Su0pTphi
    (
      const volScalarField& p,
      const volScalarField& Tu,
      scalar phi
    ) const;
    //- Laminar flame speed evaluated from the given equivalence ratio
    //  distribution corrected for temperature and pressure dependence
    tmp<volScalarField> Su0pTphi
    (
      const volScalarField& p,
      const volScalarField& Tu,
      const volScalarField& phi
    ) const;
    //- Return the Markstein number
    //  evaluated from the given equivalence ratio
    tmp<volScalarField> Ma(const volScalarField& phi) const;
    //- Construct as copy (not implemented)
    SCOPE(const SCOPE&);
    void operator=(const SCOPE&);
public:
  //- Runtime type information
  TYPE_NAME("SCOPE");
  // Constructors
    //- Construct from dictionary and psiuReactionThermo
    SCOPE
    (
      const dictionary&,
      const psiuReactionThermo&
    );
  //- Destructor
  ~SCOPE();
  // Member functions
    //- Return the Markstein number
    tmp<volScalarField> Ma() const;
    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;
};
} // End laminarFlameSpeedModels
}  // namespace mousse
#endif
