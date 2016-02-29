#ifndef THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_NSRDS_FUNCTIONS_NSRDS_FUNC7_HPP_
#define THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_NSRDS_FUNCTIONS_NSRDS_FUNC7_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc7
// Description
//   NSRDS-AICHE function number 107
//   Source:
//   \verbatim
//            NSRDS - AICHE
//         Data Compilation Tables
//           of Properties of
//           Pure Compounds
//     Design Institute for Physical Property Data
//      American Institute of Chemical Engineers
//          345 East 47th Street
//         New York, New York 10017
//     National Standard Reference Data System
//     American Institute of Chemical Engineers
//      T.E. Daubert       -       R.P. Danner
//       Department of Chemical Engineering
//       The Pennsylvania State University
//         University Park, PA 16802
//   \endverbatim
#include "thermophysical_function.hpp"
namespace mousse
{
class NSRDSfunc7
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 107 coefficients
    scalar a_, b_, c_, d_, e_;
public:
  //- Runtime type information
  TYPE_NAME("NSRDSfunc7");
  // Constructors
    //- Construct from components
    NSRDSfunc7
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e
    );
    //- Construct from Istream
    NSRDSfunc7(Istream& is);
    //- Construct from dictionary
    NSRDSfunc7(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      return a_ + b_*sqr((c_/T)/sinh(c_/T)) + d_*sqr((e_/T)/cosh(e_/T));
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << a_ << token::SPACE
        << b_ << token::SPACE
        << c_ << token::SPACE
        << d_ << token::SPACE
        << e_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc7& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
