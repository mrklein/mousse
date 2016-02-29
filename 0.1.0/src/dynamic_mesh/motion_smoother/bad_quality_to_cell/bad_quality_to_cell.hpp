#ifndef DYNAMIC_MESH_MOTION_SMOOTHER_BAD_QUALITY_TO_CELL_BAD_QUALITY_TO_CELL_HPP_
#define DYNAMIC_MESH_MOTION_SMOOTHER_BAD_QUALITY_TO_CELL_BAD_QUALITY_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::badQualityToCell
// Description
//   Selects bad quality cells (using snappyHexMesh/cvMesh mesh quality selector)
// SourceFiles
//   bad_quality_to_cell.cpp
#include "topo_set_source.hpp"
#include "packed_bool_list.hpp"
namespace mousse
{
class badQualityToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Mesh quality dictionary
    const dictionary dict_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("badQualityToCell");
  // Constructors
    //- Construct from dictionary
    badQualityToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    badQualityToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~badQualityToCell();
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
