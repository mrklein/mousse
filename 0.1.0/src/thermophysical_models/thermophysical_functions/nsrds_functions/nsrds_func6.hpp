// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc6
// Description
//   NSRDS function number 106
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
#ifndef nsrds_func6_hpp_
#define nsrds_func6_hpp_
#include "thermophysical_function.hpp"
namespace mousse
{
class NSRDSfunc6
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 106 coefficients
    scalar Tc_, a_, b_, c_, d_, e_;
public:
  //- Runtime type information
  TYPE_NAME("NSRDSfunc6");
  // Constructors
    //- Construct from components
    NSRDSfunc6
    (
      const scalar Tc,
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e
    );
    //- Construct from Istream
    NSRDSfunc6(Istream& is);
    //- Construct from dictionary
    NSRDSfunc6(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      scalar Tr = T/Tc_;
      return a_*pow(1 - Tr, ((e_*Tr + d_)*Tr + c_)*Tr + b_);
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << Tc_ << token::SPACE
        << a_ << token::SPACE
        << b_ << token::SPACE
        << c_ << token::SPACE
        << d_ << token::SPACE
        << e_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc6& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
