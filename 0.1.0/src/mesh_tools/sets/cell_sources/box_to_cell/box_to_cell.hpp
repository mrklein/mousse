// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boxToCell
// Description
//   A topoSetSource to select cells based on cell centres inside box(es).
// SourceFiles
//   box_to_cell.cpp
#ifndef box_to_cell_hpp_
#define box_to_cell_hpp_
#include "topo_set_source.hpp"
#include "tree_bound_box_list.hpp"
namespace mousse
{
class boxToCell
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
  TypeName("boxToCell");
  // Constructors
    //- Construct from components
    boxToCell
    (
      const polyMesh& mesh,
      const treeBoundBoxList& bbs
    );
    //- Construct from dictionary
    boxToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    boxToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~boxToCell();
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
