// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_time_activated_file_update_hpp_
#define io_time_activated_file_update_hpp_
#include "time_activated_file_update.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<timeActivatedFileUpdate> IOtimeActivatedFileUpdate;
}
#endif
