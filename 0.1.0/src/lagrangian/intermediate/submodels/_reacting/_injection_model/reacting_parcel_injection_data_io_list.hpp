#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_REACTING_PARCEL_INJECTION_DATA_IO_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_REACTING_PARCEL_INJECTION_DATA_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingParcelInjectionDataIOList

#include "iolist.hpp"
#include "reacting_parcel_injection_data.hpp"


namespace mousse {

typedef IOList<reactingParcelInjectionData>
  reactingParcelInjectionDataIOList;

}

#endif

