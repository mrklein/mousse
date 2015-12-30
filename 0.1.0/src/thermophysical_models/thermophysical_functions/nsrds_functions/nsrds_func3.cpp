// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func3.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(NSRDSfunc3, 0);
  addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc3, Istream);
  addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc3, dictionary);
}
// Constructors 
mousse::NSRDSfunc3::NSRDSfunc3
(
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d
)
:
  a_(a),
  b_(b),
  c_(c),
  d_(d)
{}
mousse::NSRDSfunc3::NSRDSfunc3(Istream& is)
:
  a_(readScalar(is)),
  b_(readScalar(is)),
  c_(readScalar(is)),
  d_(readScalar(is))
{}
mousse::NSRDSfunc3::NSRDSfunc3(const dictionary& dict)
:
  a_(readScalar(dict.lookup("a"))),
  b_(readScalar(dict.lookup("b"))),
  c_(readScalar(dict.lookup("c"))),
  d_(readScalar(dict.lookup("d")))
{}
