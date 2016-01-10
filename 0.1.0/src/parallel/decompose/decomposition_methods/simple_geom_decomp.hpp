// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simpleGeomDecomp
// Description
// SourceFiles
//   simple_geom_decomp.cpp
#ifndef simple_geom_decomp_hpp_
#define simple_geom_decomp_hpp_
#include "geom_decomp.hpp"
namespace mousse
{
class simpleGeomDecomp
:
  public geomDecomp
{
  // Private Member Functions
    void assignToProcessorGroup(labelList&, const label) const;
    void assignToProcessorGroup
    (
      labelList& processorGroup,
      const label nProcGroup,
      const labelList& indices,
      const scalarField& weights,
      const scalar summedWeights
    ) const;
    labelList decomposeOneProc(const pointField& points) const;
    labelList decomposeOneProc
    (
      const pointField& points,
      const scalarField& weights
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("simple");
  // Constructors
    //- Construct given the decomposition dictionary
    simpleGeomDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    simpleGeomDecomp& operator=(const simpleGeomDecomp&) = delete;
    simpleGeomDecomp(const simpleGeomDecomp&) = delete;
  //- Destructor
  virtual ~simpleGeomDecomp()
  {}
  // Member Functions
    virtual bool parallelAware() const
    {
      // simpleDecomp sends all points to the master which does
      // the decomposition.
      return true;
    }
    virtual labelList decompose(const pointField&);
    virtual labelList decompose(const pointField&, const scalarField&);
    virtual labelList decompose(const polyMesh&, const pointField& points)
    {
      return decompose(points);
    }
    virtual labelList decompose
    (
      const polyMesh&,
      const pointField& points,
      const scalarField& pointWeights
    )
    {
      return decompose(points, pointWeights);
    }
    //- Explicitly provided connectivity
    virtual labelList decompose
    (
      const labelListList& /*globalCellCells*/,
      const pointField& cc,
      const scalarField& cWeights
    )
    {
      return decompose(cc, cWeights);
    }
};
}  // namespace mousse
#endif
