#ifndef THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_THERMOPHYSICAL_FUNCTION_HPP_
#define THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_THERMOPHYSICAL_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermophysicalFunction
// Description
//   Abstract base class for thermo-physical functions

#include "scalar.hpp"
#include "iostreams.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"


namespace mousse {

class thermophysicalFunction
{
public:
  //- Runtime type information
  TYPE_NAME("thermophysicalFunction");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      thermophysicalFunction,
      Istream,
      (Istream& is),
      (is)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      thermophysicalFunction,
      dictionary,
      (const dictionary& dict),
      (dict)
    );
  // Constructors
    //- Construct null
    thermophysicalFunction()
    {}
    //- Return pointer to new thermophysicalFunction created from input
    static autoPtr<thermophysicalFunction> New(Istream& is);
    //- Return pointer to new thermophysicalFunction created from dict
    static autoPtr<thermophysicalFunction> New(const dictionary& dict);
  //- Destructor
  virtual ~thermophysicalFunction()
  {}
  // Member Functions
    //- Evaluate the function and return the result
    virtual scalar f(scalar p, scalar T) const = 0;
    //- Write the function coefficients
    virtual void writeData(Ostream& os) const = 0;
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const thermophysicalFunction& f)
    {
      f.writeData(os);
      return os;
    }
};

}  // namespace mousse

#endif

