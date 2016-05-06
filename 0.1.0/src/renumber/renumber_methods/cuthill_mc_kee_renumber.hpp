#ifndef RENUMBER_RENUMBER_METHODS_CUTHILL_MC_KEE_RENUMBER_HPP_
#define RENUMBER_RENUMBER_METHODS_CUTHILL_MC_KEE_RENUMBER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CuthillMcKeeRenumber
// Description
//   Cuthill-McKee renumbering

#include "renumber_method.hpp"
#include "switch.hpp"


namespace mousse {

class CuthillMcKeeRenumber
:
  public renumberMethod
{
  // Private data
    const Switch reverse_;
public:
  //- Runtime type information
  TYPE_NAME("CuthillMcKee");
  // Constructors
    //- Construct given the renumber dictionary
    CuthillMcKeeRenumber(const dictionary& renumberDict);
    //- Disallow default bitwise copy construct and assignment
    CuthillMcKeeRenumber& operator=(const CuthillMcKeeRenumber&) = delete;
    CuthillMcKeeRenumber(const CuthillMcKeeRenumber&) = delete;
  //- Destructor
  virtual ~CuthillMcKeeRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      NOT_IMPLEMENTED("CuthillMcKeeRenumber::renumber(const pointField&)");
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
    //  Connectivity in losort addressing (= neighbour + offsets into
    //  neighbour)
    virtual labelList renumber
    (
      const labelList& cellCells,
      const labelList& offsets,
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

