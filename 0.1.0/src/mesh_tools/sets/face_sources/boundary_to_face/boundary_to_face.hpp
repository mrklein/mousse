#ifndef MESH_TOOLS_SETS_FACE_SOURCES_BOUNDARY_TO_FACE_BOUNDARY_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_BOUNDARY_TO_FACE_BOUNDARY_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boundaryToFace
// Description
//   A topoSetSource to select all external (boundary) faces.
// SourceFiles
//   boundary_to_face.cpp
#include "topo_set_source.hpp"
namespace mousse
{
class boundaryToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("boundaryToFace");
  // Constructors
    //- Construct from components
    boundaryToFace(const polyMesh&);
    //- Construct from dictionary
    boundaryToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    boundaryToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~boundaryToFace();
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
