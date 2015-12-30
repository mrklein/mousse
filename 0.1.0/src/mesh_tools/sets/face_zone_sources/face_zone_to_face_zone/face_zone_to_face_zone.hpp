// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceZoneToFaceZone
// Description
//   A topoSetSource to select faces based on usage in another faceSet.
// SourceFiles
//   face_zone_to_face_zone.cpp
#ifndef face_zone_to_face_zone_hpp_
#define face_zone_to_face_zone_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class faceZoneToFaceZone
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
  TypeName("faceZoneToFaceZone");
  // Constructors
    //- Construct from components
    faceZoneToFaceZone
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    faceZoneToFaceZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    faceZoneToFaceZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~faceZoneToFaceZone();
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
