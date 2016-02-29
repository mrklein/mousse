#ifndef MESH_TOOLS_SETS_CELL_SOURCES_TARGET_VOLUME_TO_CELL_TARGET_VOLUME_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_TARGET_VOLUME_TO_CELL_TARGET_VOLUME_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::targetVolumeToCell
// Description
//   A topoSetSource to select cells based on the wanted volume of selected
//   cells. Adapts a plane until it has enough.
// SourceFiles
//   target_volume_to_cell.cpp
#include "topo_set_source.hpp"
#include "packed_bool_list.hpp"
namespace mousse
{
class targetVolumeToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Wanted volume
    const scalar vol_;
    //- Normal of plane to sweep
    const vector n_;
    //- Optional name of cellSet to calculate volume in
    const word maskSetName_;
  // Private Member Functions
    scalar volumeOfSet(const PackedBoolList&) const;
    label selectCells
    (
      const scalar normalComp,
      const PackedBoolList&,
      PackedBoolList& selected
    ) const;
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("targetVolumeToCell");
  // Constructors
    //- Construct from components
    targetVolumeToCell
    (
      const polyMesh& mesh,
      const scalar vol,
      const vector&
    );
    //- Construct from dictionary
    targetVolumeToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    targetVolumeToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~targetVolumeToCell();
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
