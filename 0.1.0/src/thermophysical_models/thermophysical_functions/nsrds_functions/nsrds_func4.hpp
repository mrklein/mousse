// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc4
// Description
//   NSRDS function number 104
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
#ifndef nsrds_func4_hpp_
#define nsrds_func4_hpp_
#include "thermophysical_function.hpp"
namespace mousse
{
class NSRDSfunc4
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 104 coefficients
    scalar a_, b_, c_, d_, e_;
public:
  //- Runtime type information
  TypeName("NSRDSfunc4");
  // Constructors
    //- Construct from components
    NSRDSfunc4
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e
    );
    //- Construct from Istream
    NSRDSfunc4(Istream& is);
    //- Construct from dictionary
    NSRDSfunc4(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      return a_ + b_/T + c_/pow(T, 3) + d_/pow(T, 8) + e_/pow(T, 9);
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
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc4& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
