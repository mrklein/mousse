// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::data
// Description
//   Database for solution data, solver performance and other reduced data.
//   fvMesh is derived from data so that all fields have access to the data from
//   the mesh reference they hold.
// SourceFiles
//   data.cpp

#ifndef data_hpp_
#define data_hpp_

#include "iodictionary.hpp"
#include "solver_performance.hpp"

namespace mousse
{
class data
:
  public IOdictionary
{
  // Private data

    //- Previously used time-index, used for reset between iterations
    mutable label prevTimeIndex_;

public:

  //- Debug switch
  static int debug;

  // Constructors

    //- Construct for objectRegistry
    data(const objectRegistry& obr);

    //- Disallow default bitwise copy construct
    data(const data&) = delete;

    //- Disallow default bitwise assignment
    data& operator=(const data&) = delete;

  // Member Functions

    // Access

      //- Return the dictionary of solver performance data
      //  which includes initial and final residuals for convergence
      //  checking
      const dictionary& solverPerformanceDict() const;

      //- Add/set the solverPerformance entry for the named field
      void setSolverPerformance
      (
        const word& name,
        const solverPerformance&
      ) const;

      //- Add/set the solverPerformance entry, using its fieldName
      void setSolverPerformance
      (
        const solverPerformance&
      ) const;

};

}  // namespace mousse
#endif
