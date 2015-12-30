// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointToCell
// Description
//   A topoSetSource to select cells based on usage of points.
// SourceFiles
//   point_to_cell.cpp
#ifndef point_to_cell_hpp_
#define point_to_cell_hpp_
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class pointToCell
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum pointAction
    {
      ANY,    // Cells using any point in set
      EDGE    // Cells using an edge with both points in set
      //ALL   // Possible extension: cells whose all points are in set
    };
private:
    //- Add usage string
    static addToUsageTable usage_;
    static const NamedEnum<pointAction, 2> pointActionNames_;
    //- Name of set to use
    word setName_;
    //- Option
    pointAction option_;
  // Private Member Functions
    //- Depending on point-to-cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("pointToCell");
  // Constructors
    //- Construct from components
    pointToCell
    (
      const polyMesh& mesh,
      const word& setName,
      const pointAction option
    );
    //- Construct from dictionary
    pointToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    pointToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~pointToCell();
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
