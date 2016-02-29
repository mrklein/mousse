#ifndef UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_UN_MAPPED_HPP_
#define UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_UN_MAPPED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject_list.hpp"
#include "os_specific.hpp"
namespace mousse
{
template<class Type>
void UnMapped(const IOobjectList& objects)
{
  IOobjectList fields = objects.lookupClass(Type::typeName);
  FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter)
  {
    mvBak(fieldIter()->objectPath(), "unmapped");
  }
}
}  // namespace mousse
#endif
