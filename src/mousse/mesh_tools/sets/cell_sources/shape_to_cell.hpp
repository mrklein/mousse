#ifndef MESH_TOOLS_SETS_CELL_SOURCES_SHAPE_TO_CELL_SHAPE_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_SHAPE_TO_CELL_SHAPE_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::shapeToCell
// Description
//   A topoSetSource to select cells based on cell shape.
//   Handles all ones from cellModeller and splitHex with 10 degrees
//   feature angle.

#include "topo_set_source.hpp"


namespace mousse {

class shapeToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of cell type
    word type_;
  // Private Member Functions
    //- Depending on cell type add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("shapeToCell");
  // Static data
    //- Cos of feature angle for polyHedral to be splitHex
    static scalar featureCos;
  // Constructors
    //- Construct from components
    shapeToCell
    (
      const polyMesh& mesh,
      const word& type
    );
    //- Construct from dictionary
    shapeToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    shapeToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~shapeToCell();
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
