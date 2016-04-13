#ifndef UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_TABULATED_WALL_FUNCTION_HPP_
#define UTILITIES_PRE_PROCESSING_WALL_FUNCTION_TABLE_TABULATED_WALL_FUNCTION_TABULATED_WALL_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tabulatedWallFunctions::tabulatedWallFunction
// Description
//   Base class for models that generate tabulated wall function data.
// SourceFiles
//   tabulated_wall_function.cpp

#include "dictionary.hpp"
#include "poly_mesh.hpp"
#include "run_time_selection_tables.hpp"
#include "switch.hpp"
#include "uniform_interpolation_table.hpp"

namespace mousse
{
namespace tabulatedWallFunctions
{
class tabulatedWallFunction
{
protected:
  // Proteced data
    //- Main dictionary
    const dictionary dict_;
    //- Reference to the mesh database
    const polyMesh& mesh_;
    //- Coefficients dictionary
    const dictionary coeffDict_;
    //- Name of inverted table
    word invertedTableName_;
    //- Inverted table
    uniformInterpolationTable<scalar> invertedTable_;
public:
  //- Run-time type information
  TYPE_NAME("tabulatedWallFunction");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    tabulatedWallFunction,
    dictionary,
    (
      const dictionary& dict,
      const polyMesh& mesh
    ),
    (dict, mesh)
  );
  //- Constructor
  tabulatedWallFunction
  (
    const dictionary& dict,
    const polyMesh& mesh,
    const word& name
  );
  //- Destructor
  virtual ~tabulatedWallFunction();
  //- Selector
  static autoPtr<tabulatedWallFunction> New
  (
    const dictionary& dict,
    const polyMesh& mesh
  );
  // Member Functions
    // Access
      //- Return the inverted table name
      inline const word& invertedTableName() const;
      //- Return the inverted table
      inline const uniformInterpolationTable<scalar>&
        invertedTable() const;
    // I-O
      //- Write
      virtual void write();
};
}  // namespace tabulatedWallFunctions
}  // namespace mousse

inline const mousse::word&
mousse::tabulatedWallFunctions::tabulatedWallFunction::invertedTableName() const
{
  return invertedTableName_;
}

inline const mousse::uniformInterpolationTable<mousse::scalar>&
mousse::tabulatedWallFunctions::tabulatedWallFunction::invertedTable() const
{
  return invertedTable_;
}
#endif
