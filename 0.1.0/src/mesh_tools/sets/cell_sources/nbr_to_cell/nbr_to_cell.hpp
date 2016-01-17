// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nbrToCell
// Description
//   A topoSetSource to select cells based on number of neighbouring cells
//   (i.e. number of internal or coupled faces)
// SourceFiles
//   nbr_to_cell.cpp
#ifndef nbr_to_cell_hpp_
#define nbr_to_cell_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class nbrToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Number of internal faces on cell
    label minNbrs_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("nbrToCell");
  // Constructors
    //- Construct from components
    nbrToCell
    (
      const polyMesh& mesh,
      const label minNbrs
    );
    //- Construct from dictionary
    nbrToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    nbrToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~nbrToCell();
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
