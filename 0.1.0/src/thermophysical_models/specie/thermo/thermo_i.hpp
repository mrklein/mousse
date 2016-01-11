// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo.hpp"
// Private Member Functions 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type>::thermo
(
  const Thermo& sp
)
:
  Thermo(sp)
{}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::T
(
  scalar f,
  scalar p,
  scalar T0,
  scalar (thermo<Thermo, Type>::*F)(const scalar, const scalar) const,
  scalar (thermo<Thermo, Type>::*dFdT)(const scalar, const scalar)
    const,
  scalar (thermo<Thermo, Type>::*limit)(const scalar) const
) const
{
  scalar Test = T0;
  scalar Tnew = T0;
  scalar Ttol = T0*tol_;
  int    iter = 0;
  do
  {
    Test = Tnew;
    Tnew =
      (this->*limit)
      (Test - ((this->*F)(p, Test) - f)/(this->*dFdT)(p, Test));
    if (iter++ > maxIter_)
    {
      FATAL_ERROR_IN_FUNCTION
        << "Maximum number of iterations exceeded"
        << abort(FatalError);
    }
  } while (mag(Tnew - Test) > Ttol);
  return Tnew;
}
// Constructors 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type>::thermo
(
  const word& name,
  const thermo& st
)
:
  Thermo(name, st)
{}
// Member Functions 
template<class Thermo, template<class> class Type>
inline mousse::word
mousse::species::thermo<Thermo, Type>::heName()
{
  return Type<thermo<Thermo, Type> >::name();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::he(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type> >::he(*this, p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cv(const scalar p, const scalar T) const
{
  return this->cp(p, T) - this->cpMcv(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cpv(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type> >::cpv(*this, p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::gamma(const scalar p, const scalar T) const
{
  scalar cp = this->cp(p, T);
  return cp/(cp - this->cpMcv(p, T));
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::cpBycpv
(
  const scalar p,
  const scalar T
) const
{
  return Type<thermo<Thermo, Type> >::cpBycpv(*this, p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::es(const scalar p, const scalar T) const
{
  return this->hs(p, T) - p*this->W()/this->rho(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::ea(const scalar p, const scalar T) const
{
  return this->ha(p, T) - p*this->W()/this->rho(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::g(const scalar p, const scalar T) const
{
  return this->ha(p, T) - T*this->s(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::a(const scalar p, const scalar T) const
{
  return this->ea(p, T) - T*this->s(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cpv(const scalar p, const scalar T) const
{
  return this->cpv(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cp(const scalar p, const scalar T) const
{
  return this->cp(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Cv(const scalar p, const scalar T) const
{
  return this->cv(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::HE(const scalar p, const scalar T) const
{
  return Type<thermo<Thermo, Type> >::HE(*this, p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Hs(const scalar p, const scalar T) const
{
  return this->hs(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Hc() const
{
  return this->hc()/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Ha(const scalar p, const scalar T) const
{
  return this->ha(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::S(const scalar p, const scalar T) const
{
  return this->s(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::E(const scalar p, const scalar T) const
{
  return this->e(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Es(const scalar p, const scalar T) const
{
  return this->es(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Ea(const scalar p, const scalar T) const
{
  return this->ea(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::G(const scalar p, const scalar T) const
{
  return this->g(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::A(const scalar p, const scalar T) const
{
  return this->a(p, T)/this->W();
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::K(const scalar /*p*/, const scalar T) const
{
  scalar arg = -this->nMoles()*this->g(Pstd, T)/(RR*T);
  if (arg < 600.0)
  {
    return exp(arg);
  }
  else
  {
    return VGREAT;
  }
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Kp(const scalar p, const scalar T) const
{
  return K(p, T);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar
mousse::species::thermo<Thermo, Type>::Kc(const scalar p, const scalar T) const
{
  if (equal(this->nMoles(), SMALL))
  {
    return Kp(p, T);
  }
  else
  {
    return Kp(p, T)*pow(Pstd/(RR*T), this->nMoles());
  }
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::Kx
(
  const scalar p,
  const scalar T
) const
{
  if (equal(this->nMoles(), SMALL))
  {
    return Kp(p, T);
  }
  else
  {
    return Kp(p, T)*pow(Pstd/p, this->nMoles());
  }
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::Kn
(
  const scalar p,
  const scalar T,
  const scalar n
) const
{
  if (equal(this->nMoles(), SMALL))
  {
    return Kp(p, T);
  }
  else
  {
    return Kp(p, T)*pow(n*Pstd/p, this->nMoles());
  }
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THE
(
  const scalar he,
  const scalar p,
  const scalar T0
) const
{
  return Type<thermo<Thermo, Type> >::THE(*this, he, p, T0);
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THs
(
  const scalar hs,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    hs,
    p,
    T0,
    &thermo<Thermo, Type>::Hs,
    &thermo<Thermo, Type>::Cp,
    &thermo<Thermo, Type>::limit
  );
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::THa
(
  const scalar ha,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    ha,
    p,
    T0,
    &thermo<Thermo, Type>::Ha,
    &thermo<Thermo, Type>::Cp,
    &thermo<Thermo, Type>::limit
  );
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::TEs
(
  const scalar es,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    es,
    p,
    T0,
    &thermo<Thermo, Type>::Es,
    &thermo<Thermo, Type>::Cv,
    &thermo<Thermo, Type>::limit
  );
}
template<class Thermo, template<class> class Type>
inline mousse::scalar mousse::species::thermo<Thermo, Type>::TEa
(
  const scalar ea,
  const scalar p,
  const scalar T0
) const
{
  return T
  (
    ea,
    p,
    T0,
    &thermo<Thermo, Type>::Ea,
    &thermo<Thermo, Type>::Cv,
    &thermo<Thermo, Type>::limit
  );
}
// Member Operators 
template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator+=
(
  const thermo<Thermo, Type>& st
)
{
  Thermo::operator+=(st);
}
template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator-=
(
  const thermo<Thermo, Type>& st
)
{
  Thermo::operator-=(st);
}
template<class Thermo, template<class> class Type>
inline void mousse::species::thermo<Thermo, Type>::operator*=(const scalar s)
{
  Thermo::operator*=(s);
}
// Friend Operators 
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator+
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return thermo<Thermo, Type>
  (
    static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
  );
}
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator-
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return thermo<Thermo, Type>
  (
    static_cast<const Thermo&>(st1) - static_cast<const Thermo&>(st2)
  );
}
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator*
(
  const scalar s,
  const thermo<Thermo, Type>& st
)
{
  return thermo<Thermo, Type>
  (
    s*static_cast<const Thermo&>(st)
  );
}
template<class Thermo, template<class> class Type>
inline mousse::species::thermo<Thermo, Type> mousse::species::operator==
(
  const thermo<Thermo, Type>& st1,
  const thermo<Thermo, Type>& st2
)
{
  return st2 - st1;
}
