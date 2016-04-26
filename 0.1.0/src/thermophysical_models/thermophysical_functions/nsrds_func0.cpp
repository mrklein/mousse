// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nsrds_func0.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(NSRDSfunc0, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc0, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, NSRDSfunc0, dictionary);

}


// Constructors 
mousse::NSRDSfunc0::NSRDSfunc0
(
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d,
  const scalar e,
  const scalar f
)
:
  a_{a},
  b_{b},
  c_{c},
  d_{d},
  e_{e},
  f_{f}
{}


mousse::NSRDSfunc0::NSRDSfunc0(Istream& is)
:
  a_{readScalar(is)},
  b_{readScalar(is)},
  c_{readScalar(is)},
  d_{readScalar(is)},
  e_{readScalar(is)},
  f_{readScalar(is)}
{}


mousse::NSRDSfunc0::NSRDSfunc0(const dictionary& dict)
:
  a_{readScalar(dict.lookup("a"))},
  b_{readScalar(dict.lookup("b"))},
  c_{readScalar(dict.lookup("c"))},
  d_{readScalar(dict.lookup("d"))},
  e_{readScalar(dict.lookup("e"))},
  f_{readScalar(dict.lookup("f"))}
{}

