// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingParcelInjectionDataIOList
// Description
// SourceFiles
//   reacting_parcel_injection_data_io_list.cpp
#ifndef reacting_parcel_injection_data_io_list_hpp_
#define reacting_parcel_injection_data_io_list_hpp_
#include "iolist.hpp"
#include "reacting_parcel_injection_data.hpp"
namespace mousse
{
  typedef IOList<reactingParcelInjectionData>
    reactingParcelInjectionDataIOList;
}
#endif
