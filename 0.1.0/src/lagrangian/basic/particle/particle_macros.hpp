// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef particle_macros_hpp_
#define particle_macros_hpp_
namespace mousse
{
#define DefinePropertyList(str)                                               \
                                                                              \
  static string propertyList_;                                                \
                                                                              \
  static string propertyList()                                                \
  {                                                                           \
    return str;                                                               \
  }

#define AddToPropertyList(ParcelType, str)                                    \
                                                                              \
  static string propertyList_;                                                \
                                                                              \
  static string propertyList()                                                \
  {                                                                           \
    return ParcelType::propertyList() + str;                                  \
  }

}  // namespace mousse
#endif
