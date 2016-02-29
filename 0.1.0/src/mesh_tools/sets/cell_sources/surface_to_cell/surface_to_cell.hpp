#ifndef MESH_TOOLS_SETS_CELL_SOURCES_SURFACE_TO_CELL_SURFACE_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_SURFACE_TO_CELL_SURFACE_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceToCell
// Description
//   A topoSetSource to select cells based on relation to surface.
//   Selects:
//   - all cells inside/outside/cut by surface
//   - all cells inside/outside surface ('useSurfaceOrientation', requires closed
//    surface)
//   - cells with centre nearer than XXX to surface
//   - cells with centre nearer than XXX to surface \b and with normal
//    at nearest point to centre and cell-corners differing by
//    more than YYY (i.e., point of high curvature)
// SourceFiles
//   surface_to_cell.cpp
#include "topo_set_source.hpp"
#include "map.hpp"
namespace mousse
{
class triSurfaceSearch;
class triSurface;
class surfaceToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of surface file
    const fileName surfName_;
    //- Points which are outside
    const pointField outsidePoints_;
    //- Include cut cells
    const bool includeCut_;
    //- Include inside cells
    const bool includeInside_;
    //- Include outside cells
    const bool includeOutside_;
    //- Determine inside/outside purely using geometric test
    //  (does not allow includeCut)
    const bool useSurfaceOrientation_;
    //- If > 0 : include cells with distance from cellCentre to surface
    //  less than nearDist.
    const scalar nearDist_;
    //- If > -1 : include cells with normals at nearest surface points
    //  varying more than curvature_.
    const scalar curvature_;
    //- triSurface to search on. On pointer since can be external.
    const triSurface* surfPtr_;
    //- Search engine on surface.
    const triSurfaceSearch* querySurfPtr_;
    //- Whether I allocated above surface ptrs or whether they are
    //  external.
    const bool IOwnPtrs_;
  // Private Member Functions
    //- Find index of nearest triangle to point. Returns triangle or -1 if
    //  not found within search span.
    //  Cache result under pointI.
    static label getNearest
    (
      const triSurfaceSearch& querySurf,
      const label pointI,
      const point& pt,
      const vector& searchSpan,
      Map<label>& cache
    );
    //- Return true if surface normal of nearest points to vertices on
    //  cell differ from that on cell centre. Points cached in
    //  pointToNearest.
    bool differingPointNormals
    (
      const triSurfaceSearch& querySurf,
      const vector& span,
      const label cellI,
      const label cellTriI,
      Map<label>& pointToNearest
    ) const;
    //- Depending on surface add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
    //- Check values at construction time.
    void checkSettings() const;
    const triSurfaceSearch& querySurf() const
    {
      return *querySurfPtr_;
    }
public:
  //- Runtime type information
  TYPE_NAME("surfaceToCell");
  // Constructors
    //- Construct from components
    surfaceToCell
    (
      const polyMesh& mesh,
      const fileName& surfName,
      const pointField& outsidePoints,
      const bool includeCut,
      const bool includeInside,
      const bool includeOutside,
      const bool useSurfaceOrientation,
      const scalar nearDist,
      const scalar curvature
    );
    //- Construct from components (supplied surface, surfaceSearch)
    surfaceToCell
    (
      const polyMesh& mesh,
      const fileName& surfName,
      const triSurface& surf,
      const triSurfaceSearch& querySurf,
      const pointField& outsidePoints,
      const bool includeCut,
      const bool includeInside,
      const bool includeOutside,
      const bool useSurfaceOrientation,
      const scalar nearDist,
      const scalar curvature
    );
    //- Construct from dictionary
    surfaceToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    surfaceToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~surfaceToCell();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
