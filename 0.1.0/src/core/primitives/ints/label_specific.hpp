// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
inline label sign(const label s)
{
  return (s >= 0)? 1: -1;
}
inline label pos(const label s)
{
  return (s >= 0)? 1: 0;
}
inline label neg(const label s)
{
  return (s < 0)? 1: 0;
}
inline label posPart(const label s)
{
  return (s > 0)? s: 0;
}
inline label negPart(const label s)
{
  return (s < 0)? s: 0;
}
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
