// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellToCell
// Description
//   A topoSetSource to select the cells from another cellSet.
// SourceFiles
//   cell_to_cell.cpp
#ifndef cell_to_cell_hpp_
#define cell_to_cell_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class cellToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of set to use
    word setName_;
public:
  //- Runtime type information
  TYPE_NAME("cellToCell");
  // Constructors
    //- Construct from components
    cellToCell
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    cellToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    cellToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~cellToCell();
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
