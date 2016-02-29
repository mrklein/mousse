#ifndef MESH_TOOLS_SETS_FACE_SOURCES_FACE_TO_FACE_FACE_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_FACE_TO_FACE_FACE_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceToFace
// Description
//   A topoSetSource to select faces based on usage in another faceSet.
// SourceFiles
//   face_to_face.cpp
#include "topo_set_source.hpp"
namespace mousse
{
class faceToFace
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
  TYPE_NAME("faceToFace");
  // Constructors
    //- Construct from components
    faceToFace
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    faceToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    faceToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~faceToFace();
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
