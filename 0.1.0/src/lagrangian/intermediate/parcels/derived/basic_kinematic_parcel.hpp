// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicParcel
// Description
//   Definition of basic kinematic parcel
// SourceFiles
//   basic_kinematic_parcel.hpp
#ifndef basic_kinematic_parcel_hpp_
#define basic_kinematic_parcel_hpp_
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
namespace mousse
{
  typedef KinematicParcel<particle> basicKinematicParcel;
  template<>
  inline bool contiguous<basicKinematicParcel>()
  {
    return true;
  }
}
#endif
