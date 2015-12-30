// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ptscotchDecomp
// Description
//   PTScotch domain decomposition
// SourceFiles
//   ptscotch_decomp.cpp
#ifndef ptscotch_decomp_hpp_
#define ptscotch_decomp_hpp_
#include "decomposition_method.hpp"
namespace mousse
{
class ptscotchDecomp
:
  public decompositionMethod
{
  // Private Member Functions
    //- Insert list in front of list.
    template<class Type>
    static void prepend(const UList<Type>&, List<Type>&);
    //- Insert list at end of list.
    template<class Type>
    static void append(const UList<Type>&, List<Type>&);
    //- Check and print error message
    static void check(const int, const char*);
    //- Decompose. Handles size 0 arrays
    label decompose
    (
      const fileName& meshPath,
      const List<label>& adjncy,
      const List<label>& xadj,
      const scalarField& cWeights,
      List<label>& finalDecomp
    ) const;
    //- Low level decompose
    label decompose
    (
      const fileName& meshPath,
      const label adjncySize,
      const label adjncy[],
      const label xadjSize,
      const label xadj[],
      const scalarField& cWeights,
      List<label>& finalDecomp
    ) const;
    //- Disallow default bitwise copy construct and assignment
    void operator=(const ptscotchDecomp&);
    ptscotchDecomp(const ptscotchDecomp&);
public:
  //- Runtime type information
  TypeName("ptscotch");
  // Constructors
    //- Construct given the decomposition dictionary and mesh
    ptscotchDecomp(const dictionary& decompositionDict);
  //- Destructor
  virtual ~ptscotchDecomp()
  {}
  // Member Functions
    virtual bool parallelAware() const
    {
      // ptscotch does not know about proc boundaries
      return true;
    }
    //- Inherit decompose from decompositionMethod
    using decompositionMethod::decompose;
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed). See note on weights in scotchDecomp.H
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const pointField& points,
      const scalarField& pointWeights
    );
    //- Return for every coordinate the wanted processor number. Gets
    //  passed agglomeration map (from fine to coarse cells) and coarse cell
    //  location. Can be overridden by decomposers that provide this
    //  functionality natively. See note on weights in scotchDecomp.H
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const labelList& agglom,
      const pointField& regionPoints,
      const scalarField& regionWeights
    );
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided mesh connectivity.
    //  The connectivity is equal to mesh.cellCells() except for
    //  - in parallel the cell numbers are global cell numbers (starting
    //    from 0 at processor0 and then incrementing all through the
    //    processors)
    //  - the connections are across coupled patches
    //  See note on weights in scotchDecomp.H
    virtual labelList decompose
    (
      const labelListList& globalCellCells,
      const pointField& cc,
      const scalarField& cWeights
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "ptscotch_decomp_templates.cpp"
#endif
#endif
