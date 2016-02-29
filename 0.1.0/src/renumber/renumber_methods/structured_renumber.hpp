#ifndef RENUMBER_RENUMBER_METHODS_STRUCTURED_RENUMBER_HPP_
#define RENUMBER_RENUMBER_METHODS_STRUCTURED_RENUMBER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::structuredRenumber
// Description
//   Renumbering according to mesh layers.
//   depthFirst = true:
//     first column gets ids 0..nLayer-1,
//     second nLayers..2*nLayers-1 etc.
//   depthFirst = false:
//     first layer gets ids 0,1,2 etc.
// SourceFiles
//   structured_renumber.cpp
#include "renumber_method.hpp"
#include "switch.hpp"
namespace mousse
{
class structuredRenumber
:
  public renumberMethod
{
  // Private data
    const dictionary methodDict_;
    const wordReList patches_;
    const Switch depthFirst_;
    const autoPtr<renumberMethod> method_;
    const Switch reverse_;
public:
  //- Runtime type information
  TYPE_NAME("structured");
  // Constructors
    //- Construct given the renumber dictionary
    structuredRenumber(const dictionary& renumberDict);
    //- Disallow default bitwise copy construct and assignment
    structuredRenumber& operator=(const structuredRenumber&) = delete;
    structuredRenumber(const structuredRenumber&) = delete;
  //- Destructor
  virtual ~structuredRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      NOT_IMPLEMENTED
      (
        "structuredRenumber::renumber(const pointField&)"
      );
      return labelList{0};
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
        "structuredRenumber::renumber"
        "(const labelListList&, const pointField&)"
      );
      return labelList{0};
    }
};
}  // namespace mousse
#endif
