#ifndef CORE_MATRICES_TOLERANCES_TOLERANCES_HPP_
#define CORE_MATRICES_TOLERANCES_TOLERANCES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tolerances
// Description
//   Selector class for solution tolerances.

#include "iodictionary.hpp"


namespace mousse {

class tolerances
:
  public IOdictionary
{

  // Private data

    dictionary relaxationFactors_;
    dictionary solverTolerances_;
    dictionary solverRelativeTolerances_;

public:

  // Constructors

    //- Construct from time
    tolerances(const Time& t, const fileName& dictName);

    //- Disallow default bitwise copy construct and assignment
    tolerances(const tolerances&) = delete;
    tolerances& operator=(const tolerances&) = delete;

  // Member Functions

    // Access

      bool relax(const word& name) const;
      scalar relaxationFactor(const word& name) const;
      scalar solverTolerance(const word& name) const;
      bool solverRelativeTolerances() const;
      scalar solverRelativeTolerance(const word& name) const;

    // Read

      //- Read the tolerances
      bool read();
};

}  // namespace mousse
#endif
