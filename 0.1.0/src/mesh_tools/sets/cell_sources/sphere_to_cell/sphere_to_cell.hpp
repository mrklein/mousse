// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sphereToCell
// Description
//   A topoSetSource to select cells based on cell centres inside sphere.
// SourceFiles
//   sphere_to_cell.cpp
#ifndef sphere_to_cell_hpp_
#define sphere_to_cell_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class sphereToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Centre
    vector centre_;
    //- Radius
    scalar radius_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("sphereToCell");
  // Constructors
    //- Construct from components
    sphereToCell
    (
      const polyMesh& mesh,
      const vector& centre,
      const scalar radius
    );
    //- Construct from dictionary
    sphereToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    sphereToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~sphereToCell();
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
