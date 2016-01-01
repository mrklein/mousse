// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_fields.cpp
#ifndef read_fields_hpp_
#define read_fields_hpp_
#include "ioobject_list.hpp"
#include "ptr_list.hpp"
namespace mousse
{
  // Read the fields and hold on the pointer list
  template<class Mesh, class GeoField>
  void readFields
  (
    const Mesh& mesh,
    const IOobjectList& objects,
    PtrList<GeoField>& fields
  );
}
#ifdef NoRepository
  #include "read_fields.cpp"
#endif
#endif
