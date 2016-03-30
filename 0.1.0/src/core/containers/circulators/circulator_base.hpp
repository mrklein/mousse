#ifndef CORE_CONTAINERS_CIRCULATORS_CIRCULATOR_BASE_HPP_
#define CORE_CONTAINERS_CIRCULATORS_CIRCULATOR_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CirculatorBase
// Description
//   Base class for circulators

namespace mousse {

class CirculatorBase
{
public:
  // Public data
    //- Direction type enumeration
    enum direction
    {
      NONE,
      CLOCKWISE,
      ANTICLOCKWISE
    };
  // Constructors
    //- Construct null
    CirculatorBase(){};
};

}  // namespace mousse

#endif
