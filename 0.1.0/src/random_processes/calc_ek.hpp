#ifndef RANDOM_PROCESSES_CALC_EK_HPP_
#define RANDOM_PROCESSES_CALC_EK_HPP_
#include "vol_fields_fwd.hpp"
namespace mousse
{
class graph;
class Kmesh;
graph calcEk
(
  const volVectorField& U,
  const Kmesh& K
);
}  // namespace mousse
#endif
