#ifndef CORE_MATRICES_SOLUTION_SOLUTION_HPP_
#define CORE_MATRICES_SOLUTION_SOLUTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solution
// Description
//   Selector class for relaxation factors, solver type and solution.

#include "iodictionary.hpp"


namespace mousse {

class solution
:
  public IOdictionary
{
  // Private data

    //- Dictionary of temporary fields to cache
    dictionary cache_;

    //- Switch for the caching mechanism
    bool caching_;

    //- Dictionary of relaxation factors for all the fields
    dictionary fieldRelaxDict_;

    //- Dictionary of relaxation factors for all the equations
    dictionary eqnRelaxDict_;

    //- Optional default relaxation factor for all the fields
    scalar fieldRelaxDefault_;

    //- Optional default relaxation factor for all the equations
    scalar eqnRelaxDefault_;

    //- Dictionary of solver parameters for all the fields
    dictionary solvers_;

  // Private Member Functions

    //- Read settings from the dictionary
    void read(const dictionary&);

public:

  //- Update from older solver controls syntax
  //  Usually verbose, since we want to know about the changes
  //  Returns the number of settings changed
  static label upgradeSolverDict(dictionary& dict, const bool verbose=true);

  //- Debug switch
  static int debug;

  // Constructors

    //- Construct for given objectRegistry and dictionary
    solution
    (
      const objectRegistry& obr,
      const fileName& dictName
    );

    //- Disallow default bitwise copy construct and assignment
    solution(const solution&) = delete;
    solution& operator=(const solution&) = delete;

  // Member Functions

    // Access

      //- Return true if the given field should be cached
      bool cache(const word& name) const;

      //- Helper for printing cache message
      template<class FieldType>
      static void cachePrintMessage
      (
        const char* message,
        const word& name,
        const FieldType& vf
      );

      //- Return true if the relaxation factor is given for the field
      bool relaxField(const word& name) const;

      //- Return true if the relaxation factor is given for the equation
      bool relaxEquation(const word& name) const;

      //- Return the relaxation factor for the given field
      scalar fieldRelaxationFactor(const word& name) const;

      //- Return the relaxation factor for the given eqation
      scalar equationRelaxationFactor(const word& name) const;

      //- Return the selected sub-dictionary of solvers if the "select"
      //  keyword is given, otherwise return the complete dictionary
      const dictionary& solutionDict() const;

      //- Return the solver controls dictionary for the given field
      const dictionary& solverDict(const word& name) const;

      //- Return the solver controls dictionary for the given field
      const dictionary& solver(const word& name) const;

    // Read

      //- Read the solution dictionary
      bool read();
};
}  // namespace mousse

#include "solution.ipp"

#endif
