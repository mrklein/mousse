// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef unit_conversion_hpp_
#define unit_conversion_hpp_
#include "mathematical_constants.hpp"
namespace mousse
{
//- Conversion from degrees to radians
inline scalar degToRad(const scalar deg)
{
  return (deg*constant::mathematical::pi/180.0);
}
//- Conversion from radians to degrees
inline scalar radToDeg(const scalar rad)
{
  return (rad*180.0/constant::mathematical::pi);
}
//- Conversion from atm to Pa
inline scalar atmToPa(const scalar atm)
{
  return (atm*101325.0);
}
//- Conversion from atm to Pa
inline scalar paToAtm(const scalar pa)
{
  return (pa/101325.0);
}
}  // namespace mousse
#endif
