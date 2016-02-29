#ifndef THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_NSRDS_FUNCTIONS_NSRDS_FUNC2_HPP_
#define THERMOPHYSICAL_MODELS_THERMOPHYSICAL_FUNCTIONS_NSRDS_FUNCTIONS_NSRDS_FUNC2_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc2
// Description
//   NSRDS function number 102
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
class NSRDSfunc2
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 102 coefficients
    scalar a_, b_, c_, d_;
public:
  //- Runtime type information
  TYPE_NAME("NSRDSfunc2");
  // Constructors
    //- Construct from components
    NSRDSfunc2
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d
    );
    //- Construct from Istream
    NSRDSfunc2(Istream& is);
    //- Construct from dictionary
    NSRDSfunc2(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      return a_*pow(T, b_)/(1.0 + c_/T + d_/sqr(T));
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << a_ << token::SPACE
        << b_ << token::SPACE
        << c_ << token::SPACE
        << d_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc2& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
