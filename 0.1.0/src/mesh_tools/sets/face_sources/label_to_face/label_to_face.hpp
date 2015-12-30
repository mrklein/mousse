// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelToFace
// Description
//   A topoSetSource to select faces given explicitly provided face labels.
// SourceFiles
//   label_to_face.cpp
#ifndef label_to_face_hpp_
#define label_to_face_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class labelToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Cell labels read from dictionary
    labelList labels_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("labelToFace");
  // Constructors
    //- Construct from components
    labelToFace
    (
      const polyMesh& mesh,
      const labelList& labels
    );
    //- Construct from dictionary
    labelToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    labelToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~labelToFace();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACESETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
