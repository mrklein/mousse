// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func1.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(NSRDSfunc1, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc1, Istream);
  ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc1, dictionary);
}
// Constructors 
mousse::NSRDSfunc1::NSRDSfunc1
(
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d,
  const scalar e
)
:
  a_(a),
  b_(b),
  c_(c),
  d_(d),
  e_(e)
{}
mousse::NSRDSfunc1::NSRDSfunc1(Istream& is)
:
  a_(readScalar(is)),
  b_(readScalar(is)),
  c_(readScalar(is)),
  d_(readScalar(is)),
  e_(readScalar(is))
{}
mousse::NSRDSfunc1::NSRDSfunc1(const dictionary& dict)
:
  a_(readScalar(dict.lookup("a"))),
  b_(readScalar(dict.lookup("b"))),
  c_(readScalar(dict.lookup("c"))),
  d_(readScalar(dict.lookup("d"))),
  e_(readScalar(dict.lookup("e")))
{}
