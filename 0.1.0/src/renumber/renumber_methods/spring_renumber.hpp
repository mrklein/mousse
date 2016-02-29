#ifndef RENUMBER_RENUMBER_METHODS_SPRING_RENUMBER_HPP_
#define RENUMBER_RENUMBER_METHODS_SPRING_RENUMBER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::springRenumber
// Description
//   Use spring analogy - attract neighbouring cells according to the distance
//   of their cell indices.
//   // Maximum jump of cell indices. Is fraction of number of cells
//   maxCo 0.1;
//   // Limit the amount of movement; the fraction maxCo gets decreased
//   // with every iteration.
//   freezeFraction 0.9;
//   // Maximum number of iterations
//   maxIter 1000;
// SourceFiles
//   spring_renumber.cpp
#include "renumber_method.hpp"
namespace mousse
{
class springRenumber
:
  public renumberMethod
{
  // Private data
    const dictionary& dict_;
    const scalar maxCo_;
    const label maxIter_;
    const scalar freezeFraction_;
public:
  //- Runtime type information
  TYPE_NAME("spring");
  // Constructors
    //- Construct given the renumber dictionary
    springRenumber(const dictionary& renumberDict);
    //- Disallow default bitwise copy construct and assignment
    springRenumber& operator=(const springRenumber&) = delete;
    springRenumber(const springRenumber&) = delete;
  //- Destructor
  virtual ~springRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      NOT_IMPLEMENTED("springRenumber::renumber(const pointField&)");
      return labelList(0);
    }
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  Use the mesh connectivity (if needed)
    virtual labelList renumber
    (
      const polyMesh& mesh,
      const pointField& cc
    ) const;
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  The connectivity is equal to mesh.cellCells() except
    //  - the connections are across coupled patches
    virtual labelList renumber
    (
      const labelListList& cellCells,
      const pointField& cc
    ) const;
};
}  // namespace mousse
#endif
