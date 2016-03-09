#ifndef UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_GENERAL_HPP_
#define UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_GENERAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tabulatedWallFunctions::general
// Description
//   Computes U+ as a function of Reynolds number by inverting table of
//   y+ vs U+
//   Example dictionary specification:
//     tabulatedWallFunction general;
//     // Output table info
//     tableName       uPlusWallFunctionData; // Output table name
//     log10           yes;                // Re interpreted as log10(Rey)
//     dx              0.2;                // Interval log10(Rey)
//     x0              -3;                 // Minimum log10(Rey)
//     xMax            7;                  // Maximum log10(Rey)
//     generalCoeffs
//     {
//       interpType      linear;         // Interpolation method
//       log10YPlus      true;           // y+ values defined as log10(y+)
//       log10UPlus      true;           // U+ values defined as log10(y+)
//       inputTable
//       (
//         (yPlusValue0    uPlusValue0)
//         ...
//         (yPlusValueN    uPlusValueN)
//       );
//     }
// SourceFiles
//   general.cpp

#include "tabulated_wall_function.hpp"
#include "named_enum.hpp"
#include "switch.hpp"

namespace mousse
{
namespace tabulatedWallFunctions
{
class general
:
  public tabulatedWallFunction
{
public:
  // Public data types
    //- Enumeration listing available interpolation types
    enum interpolationType
    {
      itLinear
    };
    static const NamedEnum<interpolationType, 1> interpolationTypeNames_;
protected:
  // Protected data
    //- Type of interpolation to apply when inverting the data set
    interpolationType interpType_;
    //- Input y+ values
    List<scalar> yPlus_;
    //- Input U+ values
    List<scalar> uPlus_;
    //- Are y+ values entered as log10(y+)?
    Switch log10YPlus_;
    //- Are U+ values entered as log10(U+)?
    Switch log10UPlus_;
  // Protected Member Functions
    //- Invert the table
    virtual void invertTable();
    //- Interpolate
    virtual scalar interpolate
    (
      const scalar xi,
      const scalarList& x,
      const scalarList& fx
    ) const;
public:
  //- Run-time type information
  TYPE_NAME("general");
  // Constructors
  general(const dictionary& dict, const polyMesh& mesh);
  //- Destructor
  virtual ~general();
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
