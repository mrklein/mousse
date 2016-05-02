#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TINJECTION_MODEL_REACTING_MULTIPHASE_PARCEL_INJECTION_DATA_IO_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TINJECTION_MODEL_REACTING_MULTIPHASE_PARCEL_INJECTION_DATA_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMultiphaseParcelInjectionDataIOList

#include "iolist.hpp"
#include "reacting_multiphase_parcel_injection_data.hpp"


namespace mousse {

typedef IOList<reactingMultiphaseParcelInjectionData>
  reactingMultiphaseParcelInjectionDataIOList;

}

#endif

