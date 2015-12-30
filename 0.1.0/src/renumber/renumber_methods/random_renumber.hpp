// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::randomRenumber
// Description
//   Random renumber. Just to see effect of renumbering.
// SourceFiles
//   random_renumber.cpp
#ifndef random_renumber_hpp_
#define random_renumber_hpp_
#include "renumber_method.hpp"
namespace mousse
{
class randomRenumber
:
  public renumberMethod
{
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    void operator=(const randomRenumber&);
    randomRenumber(const randomRenumber&);
public:
  //- Runtime type information
  TypeName("random");
  // Constructors
    //- Construct given the renumber dictionary
    randomRenumber(const dictionary& renumberDict);
  //- Destructor
  virtual ~randomRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const;
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
