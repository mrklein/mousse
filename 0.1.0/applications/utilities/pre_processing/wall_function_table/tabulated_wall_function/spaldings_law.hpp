#ifndef UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_SPALDINGS_LAW_HPP_
#define UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_SPALDINGS_LAW_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tabulatedWallFunctions::SpaldingsLaw
// Description
//   Computes U+ as a function of Reynolds number by inverting Spaldings law.
//   Example dictionary specification:
//     tabulatedWallFunction SpaldingsLaw;
//     // Output table info
//     tableName       uPlusWallFunctionData; // Output table name
//     log10           yes;                // Rey interpreted as log10(Rey)
//     dx              0.2;                // Interval log10(Rey)
//     x0              -3;                 // Minimum log10(Rey)
//     xMax            7;                  // Maximum log10(Rey)
//     SpaldingsLawCoeffs
//     {
//       kappa           0.41;           // Von Karman constant
//       E               9.8;            // Law-of-the-wall E coefficient
//     }

#include "tabulated_wall_function.hpp"


namespace mousse {
namespace tabulatedWallFunctions {

class SpaldingsLaw
:
  public tabulatedWallFunction
{
protected:
  // Protected data
    //- Von Karman constant
    scalar kappa_;
    //- Law-of-the-wall E coefficient
    scalar E_;
    // Newton iteration solution properties
      //- Maximum number of iterations
      static const label maxIters_;
      //- Tolerance
      static const scalar tolerance_;
  // Protected Member Functions
    //- Invert the function
    virtual void invertFunction();
public:
  //- Run-time type information
  TYPE_NAME("SpaldingsLaw");
  // Constructors
  SpaldingsLaw(const dictionary& dict, const polyMesh& mesh);
  //- Destructor
  virtual ~SpaldingsLaw();
  // Member Functions
    // Access
      //- Return y+ as a function of u+
      virtual scalar yPlus(const scalar uPlus) const;
      //- Return Reynolds number as a function of u+
      virtual scalar Re(const scalar uPlus) const;
    // I-O
      //- Write to Ostream
      virtual void writeData(Ostream& os) const;
};

}  // namespace tabulatedWallFunctions
}  // namespace mousse

#endif

