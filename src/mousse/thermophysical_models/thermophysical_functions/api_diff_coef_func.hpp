#ifndef THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_API_FUNCTIONS_API_DIFF_COEF_FUNC_HPP_
#define THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_API_FUNCTIONS_API_DIFF_COEF_FUNC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::APIdiffCoefFunc
// Description
//   API function for vapour mass diffusivity
//   Source:
//   \verbatim
//       API (American Petroleum Institute)
//           Technical Data Book
//   \endverbatim

#include "thermophysical_function.hpp"


namespace mousse {

class APIdiffCoefFunc
:
  public thermophysicalFunction
{
  // Private data
    // API vapour mass diffusivity function coefficients
    scalar a_, b_, wf_, wa_;
    // Helper variables
    scalar alpha_, beta_;
public:
  //- Runtime type information
  TYPE_NAME("APIdiffCoefFunc");
  // Constructors
    //- Construct from components
    APIdiffCoefFunc
    (
      const scalar a,
      const scalar b,
      const scalar wf,
      const scalar wa
    );
    //- Construct from Istream
    APIdiffCoefFunc(Istream& is);
    //- Construct from dictionary
    APIdiffCoefFunc(const dictionary& dict);
  // Member Functions
    //- API vapour mass diffusivity function using properties from
    //  construction
    scalar f(scalar p, scalar T) const
    {
      return 3.6059e-3*(pow(1.8*T, 1.75))*alpha_/(p*beta_);
    }
    //- API vapour mass diffusivity function using properties from
    //  construction - with specified binary pair
    scalar f(scalar p, scalar T, scalar  Wa) const
    {
      const scalar alphaBinary = sqrt(1/wf_ + 1/Wa);
      return 3.6059e-3*(pow(1.8*T, 1.75))*alphaBinary/(p*beta_);
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << a_ << token::SPACE
        << b_ << token::SPACE
        << wf_ << token::SPACE
        << wa_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const APIdiffCoefFunc& f)
    {
      f.writeData(os);
      return os;
    }
};

}  // namespace mousse

#endif

