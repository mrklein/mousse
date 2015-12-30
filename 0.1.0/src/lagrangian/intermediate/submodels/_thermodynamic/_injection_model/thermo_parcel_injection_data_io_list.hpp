// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermoParcelInjectionDataIOList
// Description
// SourceFiles
//   thermo_parcel_injection_data_io_list.cpp
#ifndef thermo_parcel_injection_data_io_list_hpp_
#define thermo_parcel_injection_data_io_list_hpp_
#include "iolist.hpp"
#include "thermo_parcel_injection_data.hpp"
namespace mousse
{
  typedef IOList<thermoParcelInjectionData>
    thermoParcelInjectionDataIOList;
}
#endif
