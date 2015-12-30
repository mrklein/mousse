// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func2.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(NSRDSfunc2, 0);
  addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc2, Istream);
  addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc2, dictionary);
}
// Constructors 
mousse::NSRDSfunc2::NSRDSfunc2
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
mousse::NSRDSfunc2::NSRDSfunc2(Istream& is)
:
  a_(readScalar(is)),
  b_(readScalar(is)),
  c_(readScalar(is)),
  d_(readScalar(is))
{}
mousse::NSRDSfunc2::NSRDSfunc2(const dictionary& dict)
:
  a_(readScalar(dict.lookup("a"))),
  b_(readScalar(dict.lookup("b"))),
  c_(readScalar(dict.lookup("c"))),
  d_(readScalar(dict.lookup("d")))
{}
