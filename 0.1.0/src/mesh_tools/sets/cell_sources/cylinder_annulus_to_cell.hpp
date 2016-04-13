#ifndef MESH_TOOLS_SETS_CELL_SOURCES_CYLINDER_ANNULUS_TO_CELL_CYLINDER_ANNULUS_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_CYLINDER_ANNULUS_TO_CELL_CYLINDER_ANNULUS_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cylinderAnnulusToCell
// Description
//   A topoSetSource to select cells based on cell centres inside a
//   cylinder annulus.

#include "topo_set_source.hpp"


namespace mousse {

class cylinderAnnulusToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- First point on cylinder axis
    vector p1_;
    //- Second point on cylinder axis
    vector p2_;
    //- Outer Radius
    scalar outerRadius_;
    //- Inner Radius
    scalar innerRadius_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("cylinderAnnulusToCell");
  // Constructors
    //- Construct from components
    cylinderAnnulusToCell
    (
      const polyMesh& mesh,
      const vector& p1,
      const vector& p2,
      const scalar outerRadius,
      const scalar innerRadius
    );
    //- Construct from dictionary
    cylinderAnnulusToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    cylinderAnnulusToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  // Destructor
  virtual ~cylinderAnnulusToCell();
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
