// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMultiphaseParcelInjectionDataIOList
// Description
// SourceFiles
//   reacting_multiphase_parcel_injection_data_io_list.cpp
#ifndef reacting_multiphase_parcel_injection_data_io_list_hpp_
#define reacting_multiphase_parcel_injection_data_io_list_hpp_
#include "iolist.hpp"
#include "reacting_multiphase_parcel_injection_data.hpp"
namespace mousse
{
  typedef IOList<reactingMultiphaseParcelInjectionData>
    reactingMultiphaseParcelInjectionDataIOList;
}
#endif
