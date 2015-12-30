// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func14.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(NSRDSfunc14, 0);
  addToRunTimeSelectionTable(thermophysicalFunction, NSRDSfunc14, Istream);
  addToRunTimeSelectionTable
  (
    thermophysicalFunction,
    NSRDSfunc14,
    dictionary
  );
}
// Constructors 
mousse::NSRDSfunc14::NSRDSfunc14
(
  const scalar Tc,
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d
)
:
  Tc_(Tc),
  a_(a),
  b_(b),
  c_(c),
  d_(d)
{}
mousse::NSRDSfunc14::NSRDSfunc14(Istream& is)
:
  Tc_(readScalar(is)),
  a_(readScalar(is)),
  b_(readScalar(is)),
  c_(readScalar(is)),
  d_(readScalar(is))
{}
mousse::NSRDSfunc14::NSRDSfunc14(const dictionary& dict)
:
  Tc_(readScalar(dict.lookup("Tc"))),
  a_(readScalar(dict.lookup("a"))),
  b_(readScalar(dict.lookup("b"))),
  c_(readScalar(dict.lookup("c"))),
  d_(readScalar(dict.lookup("d")))
{}
