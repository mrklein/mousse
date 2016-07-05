#ifndef MESH_TOOLS_SETS_CELL_SOURCES_FIELD_TO_CELL_FIELD_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_FIELD_TO_CELL_FIELD_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldToCell
// Description
//   A topoSetSource to select cells based on field values.

#include "topo_set_source.hpp"
#include "scalar_field.hpp"


namespace mousse {

class fieldToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of volScalarField
    word fieldName_;
    //- Min to subset field values with
    scalar min_;
    //- Max ,,
    scalar max_;
  // Private Member Functions
    //- Depending on field values add to or delete from cellSet.
    void applyToSet
    (
      const topoSetSource::setAction action,
      const scalarField& field,
      topoSet& set
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("fieldToCell");
  // Constructors
    //- Construct from components
    fieldToCell
    (
      const polyMesh& mesh,
      const word& fieldName,
      const scalar min,
      const scalar max
    );
    //- Construct from dictionary
    fieldToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    fieldToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~fieldToCell();
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
