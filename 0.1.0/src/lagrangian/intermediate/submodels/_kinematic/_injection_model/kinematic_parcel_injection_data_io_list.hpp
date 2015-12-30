// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kinematicParcelInjectionDataIOList
// Description
// SourceFiles
//   kinematic_parcel_injection_data_io_list.cpp
#ifndef kinematic_parcel_injection_data_io_list_hpp_
#define kinematic_parcel_injection_data_io_list_hpp_
#include "iolist.hpp"
#include "kinematic_parcel_injection_data.hpp"
namespace mousse
{
  typedef IOList<kinematicParcelInjectionData>
    kinematicParcelInjectionDataIOList;
}
#endif
