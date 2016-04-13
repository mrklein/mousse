#ifndef MESH_TOOLS_SETS_CELL_SOURCES_LABEL_TO_CELL_LABEL_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_LABEL_TO_CELL_LABEL_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelToCell
// Description
//   A topoSetSource to select cells based on explicitly given labels.

#include "topo_set_source.hpp"


namespace mousse {

class labelToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Cell labels read from dictionary
    labelList labels_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("labelToCell");
  // Constructors
    //- Construct from components
    labelToCell
    (
      const polyMesh& mesh,
      const labelList& labels
    );
    //- Construct from dictionary
    labelToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    labelToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~labelToCell();
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
