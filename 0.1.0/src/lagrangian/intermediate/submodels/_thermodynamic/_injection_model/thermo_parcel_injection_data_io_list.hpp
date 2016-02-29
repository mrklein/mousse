#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_TINJECTION_MODEL_THERMO_PARCEL_INJECTION_DATA_IO_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_TINJECTION_MODEL_THERMO_PARCEL_INJECTION_DATA_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermoParcelInjectionDataIOList
// Description
// SourceFiles
//   thermo_parcel_injection_data_io_list.cpp
#include "iolist.hpp"
#include "thermo_parcel_injection_data.hpp"
namespace mousse
{
  typedef IOList<thermoParcelInjectionData>
    thermoParcelInjectionDataIOList;
}
#endif
