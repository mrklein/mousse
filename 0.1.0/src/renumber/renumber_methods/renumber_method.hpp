#ifndef RENUMBER_RENUMBER_METHODS_RENUMBER_METHOD_HPP_
#define RENUMBER_RENUMBER_METHODS_RENUMBER_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::renumberMethod
// Description
//   Abstract base class for renumbering
// SourceFiles
//   renumber_method.cpp
#include "poly_mesh.hpp"
#include "point_field.hpp"
#include "compact_list_list.hpp"
namespace mousse
{
class renumberMethod
{
protected:
  // Protected data
    const dictionary& renumberDict_;
public:
  //- Runtime type information
  TYPE_NAME("renumberMethod");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      renumberMethod,
      dictionary,
      (
        const dictionary& renumberDict
      ),
      (renumberDict)
    );
  // Selectors
    //- Return a reference to the selected renumbering method
    static autoPtr<renumberMethod> New
    (
      const dictionary& renumberDict
    );
  // Constructors
    //- Construct given the renumber dictionary
    renumberMethod(const dictionary& renumberDict)
    :
      renumberDict_(renumberDict)
    {}
    //- Disallow default bitwise copy construct and assignment
    renumberMethod(const renumberMethod&) = delete;
    renumberMethod& operator=(const renumberMethod&) = delete;
  //- Destructor
  virtual ~renumberMethod()
  {}
  // Member Functions
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  This is only defined for geometric renumberMethods.
    virtual labelList renumber(const pointField&) const
    {
      NOT_IMPLEMENTED
      (
        "renumberMethod:renumber(const pointField&)"
      );
      return labelList(0);
    }
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  Use the mesh connectivity (if needed)
    virtual labelList renumber(const polyMesh&, const pointField&) const;
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  Addresing in losort addressing (= neighbour + offsets into
    //  neighbour)
    virtual labelList renumber
    (
      const labelList& cellCells,
      const labelList& offsets,
      const pointField&
    ) const;
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  Gets passed agglomeration map (from fine to coarse cells) and coarse
    //  cell
    //  location. Can be overridden by renumberMethods that provide this
    //  functionality natively. Coarse cells are local to the processor
    //  (if in parallel). If you want to have coarse cells spanning
    //  processors use the globalCellCells instead.
    virtual labelList renumber
    (
      const polyMesh& mesh,
      const labelList& cellToRegion,
      const pointField& regionPoints
    ) const;
    //- Return the order in which cells need to be visited, i.e.
    //  from ordered back to original cell label.
    //  The connectivity is equal to mesh.cellCells() except
    //  - the connections are across coupled patches
    virtual labelList renumber
    (
      const labelListList& cellCells,
      const pointField& cc
    ) const = 0;
};
}  // namespace mousse
#endif
