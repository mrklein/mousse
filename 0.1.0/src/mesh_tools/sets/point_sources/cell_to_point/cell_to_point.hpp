#ifndef MESH_TOOLS_SETS_POINT_SOURCES_CELL_TO_POINT_CELL_TO_POINT_HPP_
#define MESH_TOOLS_SETS_POINT_SOURCES_CELL_TO_POINT_CELL_TO_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellToPoint
// Description
//   A topoSetSource to select points based on usage in cells.
// SourceFiles
//   cell_to_point.cpp
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class cellToPoint
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum cellAction
    {
      ALL
    };
private:
    //- Add usage string
    static addToUsageTable usage_;
    static const NamedEnum<cellAction, 1> cellActionNames_;
    //- Name of set to use
    word setName_;
    //- Option
    cellAction option_;
  // Private Member Functions
    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("cellToPoint");
  // Constructors
    //- Construct from components
    cellToPoint
    (
      const polyMesh& mesh,
      const word& setName,
      const cellAction option
    );
    //- Construct from dictionary
    cellToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    cellToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~cellToPoint();
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
