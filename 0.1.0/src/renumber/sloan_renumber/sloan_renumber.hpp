// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SloanRenumber
// Description
//   Sloan renumbering algorithm
//   E.g. http://www.apav.it/sito_ratio/file_pdf/ratio_4/capitolo_1.pdf
// SourceFiles
//   sloan_renumber.cpp
#ifndef sloan_renumber_hpp_
#define sloan_renumber_hpp_
#include "renumber_method.hpp"
#include "switch.hpp"
namespace mousse
{
class SloanRenumber
:
  public renumberMethod
{
  // Private data
    const Switch reverse_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    void operator=(const SloanRenumber&);
    SloanRenumber(const SloanRenumber&);
public:
  //- Runtime type information
  TypeName("Sloan");
  // Constructors
    //- Construct given the renumber dictionary
    SloanRenumber(const dictionary& renumberDict);
  //- Destructor
  virtual ~SloanRenumber()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      notImplemented("SloanRenumber::renumber(const pointField&)");
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
