#ifndef CORE_GRAPH_CURVE_TOOLS_HPP_
#define CORE_GRAPH_CURVE_TOOLS_HPP_

#include "scalar.hpp"
#include "vector.hpp"
#include "curve.hpp"


namespace mousse {

#define curveSmall 1.0e-8
#define curveGreat 1.0e8

#include "char.hpp"
#include "list.hpp"


typedef List<char> charList;
typedef List<charList> charListList;

scalar distance(const vector&, const vector&);
bool stepForwardsToNextPoint
(
  const vector&,
  vector&,
  label&,
  label&,
  scalar,
  const curve&
);

bool stepBackwardsToNextPoint
(
  const vector&,
  vector&,
  label&,
  label&,
  scalar,
  const curve&
);

bool interpolate
(
  const vector&,
  const vector&,
  const vector&,
  vector&,
  scalar
);

bool XstepForwardsToNextPoint
(
  const vector&,
  vector&,
  label&,
  label&,
  scalar,
  const curve&
);

bool Xinterpolate
(
  const vector&,
  const vector&,
  const vector&,
  vector&,
  scalar
);

}  // namespace mousse

#endif
