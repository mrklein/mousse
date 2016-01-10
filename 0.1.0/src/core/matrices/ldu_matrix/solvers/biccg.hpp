// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BICCG
// Description
//   Diagonal incomplete LU preconditioned BiCG solver derived from the general
//   preconditioned BiCG solver PBiCG but with the choice of preconditioner
//   pre-selected.
// Deprecated
//   This solver is present for backward-compatibility and the PBiCG solver
//   should be used instead. (deprecated Apr 2008)
// SourceFiles
//   biccg.cpp

#ifndef biccg_hpp_
#define biccg_hpp_

#include "pbicg.hpp"

namespace mousse
{

class BICCG
:
  public PBiCG
{
public:

  //- Return the dictionary constructed from the components.
  //  Needed for backward compatibility
  static dictionary solverDict
  (
    const scalar tol,
    const scalar relTol
  );

  //- Return the dictionary constructed from the old-style data-stream.
  //  Needed for backward compatibility
  static dictionary solverDict(Istream&);

  //- Runtime type information
  TYPE_NAME("BICCG");

  // Constructors

    //- Construct from matrix components and solver data stream
    BICCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );

    //- Construct from matrix components and tolerances
    BICCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const scalar tolerance,
      const scalar relTol = 0.0
    );

    //- Disallow default bitwise copy construct
    BICCG(const BICCG&) = delete;

    //- Disallow default bitwise assignment
    BICCG& operator=(const BICCG&) = delete;

  //- Destructor
  virtual ~BICCG()
  {}

};

}  // namespace mousse

#endif
