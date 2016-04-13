#ifndef LAGRANGIAN_BASIC_PARTICLE_PARTICLE_MACROS_HPP_
#define LAGRANGIAN_BASIC_PARTICLE_PARTICLE_MACROS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse {

#define DEFINE_PROPERTY_LIST(str)                                             \
                                                                              \
  static string propertyList_;                                                \
                                                                              \
  static string propertyList()                                                \
  {                                                                           \
    return str;                                                               \
  }


#define ADD_TO_PROPERTY_LIST(ParcelType, str)                                 \
                                                                              \
  static string propertyList_;                                                \
                                                                              \
  static string propertyList()                                                \
  {                                                                           \
    return ParcelType::propertyList() + str;                                  \
  }

}  // namespace mousse

#endif

