// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ignition.hpp"
#include "fv_mesh.hpp"
// Member Functions 
bool mousse::ignition::igniting() const
{
  if (!ignite())
  {
    return false;
  }
  bool igning = false;
  forAll(ignSites_, i)
  {
    if (ignSites_[i].igniting())
    {
      igning = true;
    }
  }
  return igning;
}
bool mousse::ignition::ignited() const
{
  if (!ignite())
  {
    return false;
  }
  bool igned = false;
  forAll(ignSites_, i)
  {
    if (ignSites_[i].ignited())
    {
      igned = true;
    }
  }
  return igned;
}
