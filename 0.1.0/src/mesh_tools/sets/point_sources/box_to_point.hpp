#ifndef MESH_TOOLS_SETS_POINT_SOURCES_BOX_TO_POINT_BOX_TO_POINT_HPP_
#define MESH_TOOLS_SETS_POINT_SOURCES_BOX_TO_POINT_BOX_TO_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boxToPoint
// Description
//   A topoSetSource to select points based on whether they are inside box.

#include "topo_set_source.hpp"
#include "tree_bound_box_list.hpp"


namespace mousse {

class boxToPoint
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Bounding box.
    treeBoundBoxList bbs_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("boxToPoint");
  // Constructors
    //- Construct from components
    boxToPoint
    (
      const polyMesh& mesh,
      const treeBoundBoxList& bb
    );
    //- Construct from dictionary
    boxToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    boxToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~boxToPoint();
  // Member Functions
    virtual sourceType setType() const
    {
      return POINTSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
