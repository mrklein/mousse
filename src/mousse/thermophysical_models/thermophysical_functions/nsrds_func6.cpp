// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func6.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(NSRDSfunc6, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc6, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc6, dictionary);

}


// Constructors 
mousse::NSRDSfunc6::NSRDSfunc6
(
  const scalar Tc,
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d,
  const scalar e
)
:
  Tc_{Tc},
  a_{a},
  b_{b},
  c_{c},
  d_{d},
  e_{e}
{}


mousse::NSRDSfunc6::NSRDSfunc6(Istream& is)
:
  Tc_{readScalar(is)},
  a_{readScalar(is)},
  b_{readScalar(is)},
  c_{readScalar(is)},
  d_{readScalar(is)},
  e_{readScalar(is)}
{}


mousse::NSRDSfunc6::NSRDSfunc6(const dictionary& dict)
:
  Tc_{readScalar(dict.lookup("Tc"))},
  a_{readScalar(dict.lookup("a"))},
  b_{readScalar(dict.lookup("b"))},
  c_{readScalar(dict.lookup("c"))},
  d_{readScalar(dict.lookup("d"))},
  e_{readScalar(dict.lookup("e"))}
{}

