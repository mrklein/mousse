// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::badQualityToFace
// Description
//   Selects bad quality faces (using snappyHexMesh/cvMesh mesh quality selector)
// SourceFiles
//   bad_quality_to_face.cpp
#ifndef bad_quality_to_face_hpp_
#define bad_quality_to_face_hpp_
#include "topo_set_source.hpp"
#include "packed_bool_list.hpp"
namespace mousse
{
class badQualityToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Mesh quality dictionary
    const dictionary dict_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("badQualityToFace");
  // Constructors
    //- Construct from dictionary
    badQualityToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    badQualityToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~badQualityToFace();
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
