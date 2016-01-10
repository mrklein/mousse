// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hierarchGeomDecomp
// Description
//   Does hierarchical decomposition of points. Works by first sorting the
//   points in x direction into equal sized bins, then in y direction and
//   finally in z direction.
//   Uses single array to hold decomposition which is indexed as if it is a
//   3 dimensional array:
//     finalDecomp[i,j,k] is indexed as
//     i*n[0]*n[1] + j*n[1] + k
//   E.g. if we're sorting 'xyz': the first sort (over the x-component)
//   determines in which x-domain the point goes. Then for each of the x-domains
//   the points are sorted in y direction and each individual x-domain gets
//   split into three y-domains. And similar for the z-direction.
//   Since the domains are of equal size the maximum difference in size is
//   n[0]*n[1] (or n[1]*n[2]?) (small anyway)
// SourceFiles
//   hierarch_geom_decomp.cpp
#ifndef hierarch_geom_decomp_hpp_
#define hierarch_geom_decomp_hpp_
#include "geom_decomp.hpp"
#include "fixed_list.hpp"
#include "direction.hpp"
namespace mousse
{
class hierarchGeomDecomp
:
  public geomDecomp
{
  // Private data
    //- Decomposition order in terms of components.
    FixedList<direction, 3> decompOrder_;
  // Private Member Functions
    //- Convert ordering string ("xyz") into list of components.
    void setDecompOrder();
    //- Evaluates the weighted sizes for each sorted point.
    static void calculateSortedWeightedSizes
    (
      const labelList& current,
      const labelList& indices,
      const scalarField& weights,
      const label globalCurrentSize,
      scalarField& sortedWeightedSizes
    );
    //- Find index of t in list inbetween indices left and right
    static label findLower
    (
      const List<scalar>&,
      const scalar t,
      const label left,
      const label right
    );
    //- Find midValue (at local index mid) such that the number of
    //  elements between mid and leftIndex are (globally summed) the
    //  wantedSize. Binary search.
    static void findBinary
    (
      const label sizeTol,        // size difference considered acceptible
      const List<scalar>&,
      const label leftIndex,      // index of previous value
      const scalar leftValue,     // value at leftIndex
      const scalar maxValue,      // global max of values
      const scalar wantedSize,    // wanted size
      label& mid,                 // index where size of bin is wantedSize
      scalar& midValue            // value at mid
    );
    //- Find midValue (at local index mid) such that the number of
    //  elements between mid and leftIndex are (globally summed) the
    //  wantedSize. Binary search.
    static void findBinary
    (
      const label sizeTol,        // size difference considered acceptible
      const List<scalar>& sortedWeightedSizes,
      const List<scalar>&,
      const label leftIndex,      // index of previous value
      const scalar leftValue,     // value at leftIndex
      const scalar maxValue,      // global max of values
      const scalar wantedSize,    // wanted size
      label& mid,                 // index where size of bin is wantedSize
      scalar& midValue            // value at mid
    );
    //- Recursively sort in x,y,z (or rather acc. to decompOrder_)
    void sortComponent
    (
      const label sizeTol,
      const pointField&,
      const labelList& slice,         // slice of points to decompose
      const direction componentIndex, // index in decompOrder_
      const label prevMult,           // multiplication factor
      labelList& finalDecomp          // overall decomposition
    );
    //- Recursively sort in x,y,z (or rather acc. to decompOrder_)
    //- Using weighted points.
    void sortComponent
    (
      const label sizeTol,
      const scalarField& weights,
      const pointField&,
      const labelList& slice,         // slice of points to decompose
      const direction componentIndex, // index in decompOrder_
      const label prevMult,           // multiplication factor
      labelList& finalDecomp          // overall decomposition
    );
public:
  //- Runtime type information
  TYPE_NAME("hierarchical");
  // Constructors
    //- Construct given the decomposition dictionary
    hierarchGeomDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    hierarchGeomDecomp& operator=(const hierarchGeomDecomp&) = delete;
    hierarchGeomDecomp(const hierarchGeomDecomp&) = delete;
  //- Destructor
  virtual ~hierarchGeomDecomp()
  {}
  // Member Functions
    //- Hierarchgeom is aware of processor boundaries
    virtual bool parallelAware() const
    {
      return true;
    }
    //- Return for every coordinate the wanted processor number.
    virtual labelList decompose
    (
      const pointField&,
      const scalarField& weights
    );
    //- Without weights. Code for weighted decomposition is a bit complex
    //  so kept separate for now.
    virtual labelList decompose(const pointField&);
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed)
    virtual labelList decompose
    (
      const polyMesh&,
      const pointField& cc,
      const scalarField& cWeights
    )
    {
      return decompose(cc, cWeights);
    }
    //- Without weights. Code for weighted decomposition is a bit complex
    //  so kept separate for now.
    virtual labelList decompose(const polyMesh&, const pointField& cc)
    {
      return decompose(cc);
    }
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided connectivity - does not use mesh_.
    //  The connectivity is equal to mesh.cellCells() except for
    //  - in parallel the cell numbers are global cell numbers (starting
    //    from 0 at processor0 and then incrementing all through the
    //    processors)
    //  - the connections are across coupled patches
    virtual labelList decompose
    (
      const labelListList& /*globalCellCells*/,
      const pointField& cc,
      const scalarField& cWeights
    )
    {
      return decompose(cc, cWeights);
    }
    virtual labelList decompose
    (
      const labelListList& /*globalCellCells*/,
      const pointField& cc
    )
    {
      return decompose(cc);
    }
};
}  // namespace mousse
#endif
