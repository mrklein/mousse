#ifndef RANDOM_PROCESSES_EK_HPP_
#define RANDOM_PROCESSES_EK_HPP_

namespace mousse {

inline tmp<scalarField> Ek
(
  const scalar Ea,
  const scalar k0,
  const scalarField& k
)
{
  tmp<scalarField> tEk(Ea*pow(k/k0, 4.0)*exp(-2.0*sqr(k/k0)));
  return tEk;
}

}  // namespace mousse

#endif

