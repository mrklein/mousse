// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelToPoint
// Description
//   A topoSetSource to select points given explicitly provided labels.
// SourceFiles
//   label_to_point.cpp
#ifndef label_to_point_hpp_
#define label_to_point_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class labelToPoint
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Point labels read from dictionary
    labelList labels_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("labelToPoint");
  // Constructors
    //- Construct from components
    labelToPoint
    (
      const polyMesh& mesh,
      const labelList& labels
    );
    //- Construct from dictionary
    labelToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    labelToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~labelToPoint();
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
