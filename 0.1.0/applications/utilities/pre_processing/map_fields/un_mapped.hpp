// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef un_mapped_hpp_
#define un_mapped_hpp_
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
