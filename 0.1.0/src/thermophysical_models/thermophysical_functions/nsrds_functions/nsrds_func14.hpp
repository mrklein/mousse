// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc14
// Description
//   NSRDS function number 114
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
#ifndef nsrds_func14_hpp_
#define nsrds_func14_hpp_
#include "thermophysical_function.hpp"
namespace mousse
{
class NSRDSfunc14
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 114 coefficients
    scalar Tc_, a_, b_, c_, d_;
public:
  //- Runtime type information
  TYPE_NAME("NSRDSfunc14");
  // Constructors
    //- Construct from components
    NSRDSfunc14
    (
      const scalar Tc,
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d
    );
    //- Construct from Istream
    NSRDSfunc14(Istream& is);
    //- Construct from dictionary
    NSRDSfunc14(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      scalar Tdash = min(T, Tc_ - ROOTVSMALL);
      scalar t = 1.0 - Tdash/Tc_;
      return
        a_*a_/(t + ROOTVSMALL) + b_ - t
       *(
         2.0*a_*c_
        + t*(a_*d_ + t*(c_*c_/3.0 + t*(0.5*c_*d_ + 0.2*d_*d_*t)))
        );
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << Tc_ << token::SPACE
        << a_ << token::SPACE
        << b_ << token::SPACE
        << c_ << token::SPACE
        << d_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc14& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
