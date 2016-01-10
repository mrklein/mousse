// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setToFaceZone
// Description
//   A topoSetSource to select faces based on usage in a faceSet. Sets flipMap
//   to true.
// SourceFiles
//   set_to_face_zone.cpp
#ifndef set_to_face_zone_hpp_
#define set_to_face_zone_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class setToFaceZone
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
  TYPE_NAME("setToFaceZone");
  // Constructors
    //- Construct from components
    setToFaceZone
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    setToFaceZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    setToFaceZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~setToFaceZone();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACEZONESOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
