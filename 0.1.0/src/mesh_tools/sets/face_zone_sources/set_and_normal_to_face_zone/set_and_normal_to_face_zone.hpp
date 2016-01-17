// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setAndNormalToFaceZone
// Description
//   A topoSetSource to select faces based on usage in a faceSet, where the
//   normal vector is used to orient the faces.
// SourceFiles
//   set_and_normal_to_face_zone.cpp
#ifndef set_and_normal_to_face_zone_hpp_
#define set_and_normal_to_face_zone_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class setAndNormalToFaceZone
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of set to use
    word setName_;
    //- Normal used to orient the faces
    vector normal_;
public:
  //- Runtime type information
  TYPE_NAME("setAndNormalToFaceZone");
  // Constructors
    //- Construct from components
    setAndNormalToFaceZone
    (
      const polyMesh& mesh,
      const word& setName,
      const vector& normal
    );
    //- Construct from dictionary
    setAndNormalToFaceZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    setAndNormalToFaceZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~setAndNormalToFaceZone();
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
