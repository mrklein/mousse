#ifndef RENUMBER_RENUMBER_METHODS_MANUAL_RENUMBER_HPP_
#define RENUMBER_RENUMBER_METHODS_MANUAL_RENUMBER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::manualRenumber
// Description
//   Renumber given a ordered-to-original cell association in a file

#include "renumber_method.hpp"


namespace mousse {

class manualRenumber
:
  public renumberMethod
{
  // Private data
    const fileName dataFile_;
public:
  //- Runtime type information
  TYPE_NAME("manual");
  // Constructors
    //- Construct given the renumber dictionary
    manualRenumber(const dictionary& renumberDict);
    //- Disallow default bitwise copy construct and assignment
    manualRenumber& operator=(const manualRenumber&) = delete;
    manualRenumber(const manualRenumber&) = delete;
  //- Destructor
  virtual ~manualRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      NOT_IMPLEMENTED("manualRenumber::renumber(const pointField&)");
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
      const labelListList& /*cellCells*/,
      const pointField& /*cc*/
    ) const
    {
      NOT_IMPLEMENTED
      (
        "manualRenumber::renumber"
        "(const labelListList&, const pointField&)"
      );
      return labelList{0};
    }
};

}  // namespace mousse

#endif

