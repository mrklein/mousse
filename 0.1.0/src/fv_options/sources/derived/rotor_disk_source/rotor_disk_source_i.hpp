// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotor_disk_source.hpp"
mousse::scalar mousse::fv::rotorDiskSource::rhoRef() const
{
  return rhoRef_;
}
mousse::scalar mousse::fv::rotorDiskSource::omega() const
{
  return omega_;
}
const mousse::List<mousse::point>& mousse::fv::rotorDiskSource::x() const
{
  return x_;
}
const mousse::cylindricalCS& mousse::fv::rotorDiskSource::coordSys() const
{
  return coordSys_;
}
