#ifndef peng_robinson_gas_hpp_
#define peng_robinson_gas_hpp_
#include "auto_ptr.hpp"
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
#include "peng_robinson_gas_i.hpp"
#ifdef NoRepository
#   include "peng_robinson_gas.cpp"
#endif
#endif
