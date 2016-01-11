// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NSRDSfunc0
// Description
//   NSRDS function number 100
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
#ifndef nsrds_func0_hpp_
#define nsrds_func0_hpp_
#include "thermophysical_function.hpp"
namespace mousse
{
class NSRDSfunc0
:
  public thermophysicalFunction
{
  // Private data
    // NSRDS function 100 coefficients
    scalar a_, b_, c_, d_, e_, f_;
public:
  //- Runtime type information
  TYPE_NAME("NSRDSfunc0");
  // Constructors
    //- Construct from components
    NSRDSfunc0
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e,
      const scalar f
    );
    //- Construct from Istream
    NSRDSfunc0(Istream& is);
    //- Construct from dictionary
    NSRDSfunc0(const dictionary& dict);
  // Member Functions
    //- Evaluate the function and return the result
    scalar f(scalar, scalar T) const
    {
      return ((((f_*T + e_)*T + d_)*T + c_)*T + b_)*T + a_;
    }
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      os  << a_ << token::SPACE
        << b_ << token::SPACE
        << c_ << token::SPACE
        << d_ << token::SPACE
        << e_ << token::SPACE
        << f_;
    }
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const NSRDSfunc0& f)
    {
      f.writeData(os);
      return os;
    }
};
}  // namespace mousse
#endif
