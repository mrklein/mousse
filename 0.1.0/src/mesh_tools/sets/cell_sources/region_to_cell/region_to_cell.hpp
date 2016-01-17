// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionToCell
// Description
//   TopoSetSource. Select cells belonging to topological connected region
//   (that contains given points)
//   In dictionary input:
//     // optional name of cellSet delimiting search
//     set         c0;
//     //- Number of cell layers to erode mesh to detect holes in the mesh
//     //  Set to 0 if not used.
//     nErode 3;
//     // points inside region to select
//     insidePoints ((1 2 3));
// SourceFiles
//   region_to_cell.cpp
#ifndef region_to_cell_hpp_
#define region_to_cell_hpp_
#include "topo_set_source.hpp"
#include "bool_list.hpp"
namespace mousse
{
class regionSplit;
class regionToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of cellSet to keep to
    const word setName_;
    //- Coordinate(s) that is inside connected region
    const pointField insidePoints_;
    //- Number of layers to erode
    const label nErode_;
  // Private Member Functions
    //- Mark faces inbetween selected and unselected elements
    void markRegionFaces
    (
      const boolList& selectedCell,
      boolList& regionFace
    ) const;
    //- Determine for every disconnected region in the mesh whether
    //  it contains a locationInMesh
    boolList findRegions(const bool verbose, const regionSplit&) const;
    //- Unselect regions not containing a locationInMesh
    void unselectOutsideRegions(boolList& selectedCell) const;
    //- Unselect one layer of cells from selectedCell
    void shrinkRegions(boolList& selectedCell) const;
    //- Erode a given number of layers from selectedCell. Remove any
    //  region that gets disconnected that way.
    void erode(boolList& selectedCell) const;
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("regionToCell");
  // Constructors
    //- Construct from components
    regionToCell
    (
      const polyMesh& mesh,
      const word& setName,
      const pointField& insidePoints,
      const label nErode
    );
    //- Construct from dictionary
    regionToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    regionToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~regionToCell();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLSETSOURCE;
    }
    virtual void applyToSet(const topoSetSource::setAction action, topoSet&)
    const;
};
}  // namespace mousse
#endif
