// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "api_diff_coef_func.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(APIdiffCoefFunc, 0);
  addToRunTimeSelectionTable
  (
    thermophysicalFunction,
    APIdiffCoefFunc,
    Istream
  );
  addToRunTimeSelectionTable
  (
    thermophysicalFunction,
    APIdiffCoefFunc,
    dictionary
  );
}
// Constructors 
mousse::APIdiffCoefFunc::APIdiffCoefFunc
(
  const scalar a,
  const scalar b,
  const scalar wf,
  const scalar wa
)
:
  a_(a),
  b_(b),
  wf_(wf),
  wa_(wa),
  alpha_(sqrt(1/wf_ + 1/wa_)),
  beta_(sqr(cbrt(a_) + cbrt(b_)))
{}
mousse::APIdiffCoefFunc::APIdiffCoefFunc(Istream& is)
:
  a_(readScalar(is)),
  b_(readScalar(is)),
  wf_(readScalar(is)),
  wa_(readScalar(is)),
  alpha_(sqrt(1/wf_ + 1/wa_)),
  beta_(sqr((cbrt(a_) + cbrt(b_))))
{}
mousse::APIdiffCoefFunc::APIdiffCoefFunc(const dictionary& dict)
:
  a_(readScalar(dict.lookup("a"))),
  b_(readScalar(dict.lookup("b"))),
  wf_(readScalar(dict.lookup("wf"))),
  wa_(readScalar(dict.lookup("wa"))),
  alpha_(sqrt(1/wf_ + 1/wa_)),
  beta_(sqr((cbrt(a_) + cbrt(b_))))
{}
