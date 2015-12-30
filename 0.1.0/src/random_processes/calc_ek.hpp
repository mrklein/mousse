#ifndef calc_ek_hpp_
#define calc_ek_hpp_
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
