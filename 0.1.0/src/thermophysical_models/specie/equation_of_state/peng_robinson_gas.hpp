#ifndef peng_robinson_gas_hpp_
#define peng_robinson_gas_hpp_
#include "auto_ptr.hpp"
#include "mathematical_constants.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Specie> class PengRobinsonGas;
template<class Specie>
inline PengRobinsonGas<Specie> operator+
(
  const PengRobinsonGas<Specie>&,
  const PengRobinsonGas<Specie>&
);
template<class Specie>
inline PengRobinsonGas<Specie> operator-
(
  const PengRobinsonGas<Specie>&,
  const PengRobinsonGas<Specie>&
);
template<class Specie>
inline PengRobinsonGas<Specie> operator*
(
  const scalar,
  const PengRobinsonGas<Specie>&
);
template<class Specie>
inline PengRobinsonGas<Specie> operator==
(
  const PengRobinsonGas<Specie>&,
  const PengRobinsonGas<Specie>&
);
template<class Specie>
Ostream& operator<<
(
  Ostream&,
  const PengRobinsonGas<Specie>&
);
template<class Specie>
class PengRobinsonGas
:
  public Specie
{
  // Private data
    //- Critical Temperature [K]
    scalar Tc_;
    //- Critical volume [m^3/kmol]
    scalar Vc_;
    //- Critical compression factor [-]
    scalar Zc_;
    //- Critical Pressure [Pa]
    scalar Pc_;
    //- Acentric factor [-]
    scalar omega_;
public:
  // Constructors
    //- Construct from components
    inline PengRobinsonGas
    (
      const Specie& sp,
      const scalar& Tc,
      const scalar& Vc,
      const scalar& Zc,
      const scalar& Pc,
      const scalar& omega
    );
    //- Construct from Istream
    PengRobinsonGas(Istream&);
    //- Construct from dictionary
    PengRobinsonGas(const dictionary& dict);
    //- Construct as named copy
    inline PengRobinsonGas(const word& name, const PengRobinsonGas&);
    //- Construct and return a clone
    inline autoPtr<PengRobinsonGas> clone() const;
    // Selector from Istream
    inline static autoPtr<PengRobinsonGas> New(Istream& is);
    // Selector from dictionary
    inline static autoPtr<PengRobinsonGas> New
    (
      const dictionary& dict
    );
  // Member functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "PengRobinsonGas<" + word(Specie::typeName_()) + '>';
    }
    // Fundamental properties
      //- Is the equation of state is incompressible i.e. rho != f(p)
      static const bool incompressible = false;
      //- Is the equation of state is isochoric i.e. rho = const
      static const bool isochoric = false;
      //- Return density [kg/m^3]
      inline scalar rho(scalar p, scalar T) const;
      //- Return entropy [J/(kmol K)]
      inline scalar s(const scalar p, const scalar T) const;
      //- Return compressibility rho/p [s^2/m^2]
      inline scalar psi(scalar p, scalar T) const;
      //- Return compression factor [-]
      inline scalar Z(scalar p, scalar T) const;
      //- Return (cp - cv) [J/(kmol K]
      inline scalar cpMcv(scalar p, scalar T) const;
  // IO
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline void operator+=(const PengRobinsonGas&);
    inline void operator-=(const PengRobinsonGas&);
    inline void operator*=(const scalar);
  // Friend operators
    friend PengRobinsonGas operator+ <Specie>
    (
      const PengRobinsonGas&,
      const PengRobinsonGas&
    );
    friend PengRobinsonGas operator- <Specie>
    (
      const PengRobinsonGas&,
      const PengRobinsonGas&
    );
    friend PengRobinsonGas operator* <Specie>
    (
      const scalar s,
      const PengRobinsonGas&
    );
    friend PengRobinsonGas operator== <Specie>
    (
      const PengRobinsonGas&,
      const PengRobinsonGas&
    );
  // Ostream Operator
    friend Ostream& operator<< <Specie>
    (
      Ostream&,
      const PengRobinsonGas&
    );
};
}  // namespace mousse

// Private Member Functions 
template<class Specie>
inline mousse::PengRobinsonGas<Specie>::PengRobinsonGas
(
  const Specie& sp,
  const scalar& Tc,
  const scalar& Vc,
  const scalar& Zc,
  const scalar& Pc,
  const scalar& omega
)
:
  Specie{sp},
  Tc_{Tc},
  Vc_{Vc},
  Zc_{Zc},
  Pc_{Pc},
  omega_{omega}
{}
// Constructors 
template<class Specie>
inline mousse::PengRobinsonGas<Specie>::PengRobinsonGas
(
  const word& name,
  const PengRobinsonGas& pg
)
:
  Specie{name, pg},
  Tc_{pg.Tc_},
  Pc_{pg.Pc_},
  Vc_{pg.Vc_},
  Zc_{pg.Zc_},
  omega_{pg.omega_}
{}
template<class Specie>
inline mousse::autoPtr<mousse::PengRobinsonGas <Specie> >
mousse::PengRobinsonGas<Specie>::clone() const
{
  return {new PengRobinsonGas<Specie>{*this}};
}
template<class Specie>
inline mousse::autoPtr<mousse::PengRobinsonGas<Specie> >
mousse::PengRobinsonGas<Specie>::New
(
  Istream& is
)
{
  return {new PengRobinsonGas<Specie>{is}};
}
template<class Specie>
inline mousse::autoPtr<mousse::PengRobinsonGas<Specie> >
mousse::PengRobinsonGas<Specie>::New
(
  const dictionary& dict
)
{
  return {new PengRobinsonGas<Specie>(dict)};
}
// Member Functions 
template<class Specie>
inline mousse::scalar mousse::PengRobinsonGas<Specie>::rho
(
  scalar p,
  scalar T
) const
{
  scalar z = Z(p, T);
  return p/(z*this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::PengRobinsonGas<Specie>::s
(
  scalar p,
  scalar /*T*/
) const
{
  //***HGW This is the expression for a perfect gas
  // Need to add the entropy defect for Peng-Robinson
  return -RR*log(p/Pstd);
}
template<class Specie>
inline mousse::scalar mousse::PengRobinsonGas<Specie>::psi
(
  scalar p,
  scalar T
) const
{
  scalar z = Z(p, T);
  return 1.0/(z*this->R()*T);
}
template<class Specie>
inline mousse::scalar mousse::PengRobinsonGas<Specie>::Z
(
  scalar p,
  scalar T
) const
{
  scalar a = 0.45724*sqr(this->R())*sqr(Tc_)/Pc_;
  scalar b = 0.07780*this->R()*Tc_/Pc_;
  scalar Tr = T/Tc_;
  scalar alpha =
    sqr
    (
      1.0
     + (0.37464 + 1.54226*omega_- 0.26992*sqr(omega_))
     * (1.0 - sqrt(Tr))
    );
  scalar B = b*p/(this->R()*T);
  scalar A = a*alpha*p/sqr(this->R()*T);
  scalar a2 = B - 1.0;
  scalar a1 = A - 2.0*B - 3.0*sqr(B);
  scalar a0 = -A*B + sqr(B) + pow3(B);
  scalar Q = (3.0*a1 - a2*a2)/9.0;
  scalar Rl = (9.0*a2*a1 - 27.0*a0 - 2.0*a2*a2*a2)/54;
  scalar Q3 = Q*Q*Q;
  scalar D = Q3 + Rl*Rl;
  scalar root = -1.0;
  if (D <= 0.0)
  {
    scalar th = ::acos(Rl/sqrt(-Q3));
    scalar qm = 2.0*sqrt(-Q);
    scalar r1 = qm*cos(th/3.0) - a2/3.0;
    scalar r2 = qm*cos((th + 2.0*constant::mathematical::pi)/3.0) - a2/3.0;
    scalar r3 = qm*cos((th + 4.0*constant::mathematical::pi)/3.0) - a2/3.0;
    root = max(r1, max(r2, r3));
  }
  else
  {
    // one root is real
    scalar D05 = sqrt(D);
    scalar S = pow(Rl + D05, 1.0/3.0);
    scalar Tl = 0;
    if (D05 > Rl)
    {
      Tl = -pow(mag(Rl - D05), 1.0/3.0);
    }
    else
    {
      Tl = pow(Rl - D05, 1.0/3.0);
    }
    root = S + Tl - a2/3.0;
  }
  return root;
}
template<class Specie>
inline mousse::scalar mousse::PengRobinsonGas<Specie>::cpMcv
(
  scalar p,
  scalar T
) const
{
  return RR*Z(p, T);
}
// Member Operators 
template<class Specie>
inline void mousse::PengRobinsonGas<Specie>::operator+=
(
  const PengRobinsonGas<Specie>& pg
)
{
  scalar molr1 = this->nMoles();
  Specie::operator+=(pg);
  molr1 /= this->nMoles();
  scalar molr2 = pg.nMoles()/this->nMoles();
  Tc_ = molr1*Tc_ + molr2*pg.Tc_;
  Vc_ = molr1*Vc_ + molr2*pg.Vc_;
  Zc_ = molr1*Zc_ + molr2*pg.Zc_;
  Pc_ = RR*Zc_*Tc_/Vc_;
  omega_ = molr1*omega_ + molr2*pg.omega_;
}
template<class Specie>
inline void mousse::PengRobinsonGas<Specie>::operator-=
(
  const PengRobinsonGas<Specie>& pg
)
{
  scalar molr1 = this->nMoles();
  Specie::operator-=(pg);
  molr1 /= this->nMoles();
  scalar molr2 = pg.nMoles()/this->nMoles();
  Tc_ = molr1*Tc_ - molr2*pg.Tc_;
  Vc_ = molr1*Vc_ - molr2*pg.Vc_;
  Zc_ = molr1*Zc_ - molr2*pg.Zc_;
  Pc_ = RR*Zc_*Tc_/Vc_;
  omega_ = molr1*omega_ - molr2*pg.omega_;
}
template<class Specie>
inline void mousse::PengRobinsonGas<Specie>::operator*=(const scalar s)
{
  Specie::operator*=(s);
}
// Friend Operators 
template<class Specie>
mousse::PengRobinsonGas<Specie> mousse::operator+
(
  const PengRobinsonGas<Specie>& pg1,
  const PengRobinsonGas<Specie>& pg2
)
{
  scalar nMoles = pg1.nMoles() + pg2.nMoles();
  scalar molr1 = pg1.nMoles()/nMoles;
  scalar molr2 = pg2.nMoles()/nMoles;
  const scalar Tc = molr1*pg1.Tc_ + molr2*pg2.Tc_;
  const scalar Vc = molr1*pg1.Vc_ + molr2*pg2.Vc_;
  const scalar Zc = molr1*pg1.Zc_ + molr2*pg2.Zc_;
  return {static_cast<const Specie&>(pg1) + static_cast<const Specie&>(pg2),
          Tc,
          Vc,
          Zc,
          RR*Zc*Tc/Vc,
          molr1*pg1.omega_ + molr2*pg2.omega_};
}
template<class Specie>
mousse::PengRobinsonGas<Specie> mousse::operator-
(
  const PengRobinsonGas<Specie>& pg1,
  const PengRobinsonGas<Specie>& pg2
)
{
  scalar nMoles = pg1.nMoles() + pg2.nMoles();
  scalar molr1 = pg1.nMoles()/nMoles;
  scalar molr2 = pg2.nMoles()/nMoles;
  const scalar Tc = molr1*pg1.Tc_ + molr2*pg2.Tc_;
  const scalar Vc = molr1*pg1.Vc_ + molr2*pg2.Vc_;
  const scalar Zc = molr1*pg1.Zc_ + molr2*pg2.Zc_;
  return {static_cast<const Specie&>(pg1) - static_cast<const Specie&>(pg2),
          Tc,
          Vc,
          Zc,
          RR*Zc*Tc/Vc,
          molr1*pg1.omega_ - molr2*pg2.omega_};
}
template<class Specie>
mousse::PengRobinsonGas<Specie> mousse::operator*
(
  const scalar s,
  const PengRobinsonGas<Specie>& pg
)
{
  return {s*static_cast<const Specie&>(pg),
          pg.Tc_,
          pg.Vc_,
          pg.Zc_,
          pg.Pc_,
          pg.omega_};
}
template<class Specie>
mousse::PengRobinsonGas<Specie> mousse::operator==
(
  const PengRobinsonGas<Specie>& pg1,
  const PengRobinsonGas<Specie>& pg2
)
{
  return pg2 - pg1;
}
#ifdef NoRepository
#   include "peng_robinson_gas.cpp"
#endif
#endif
