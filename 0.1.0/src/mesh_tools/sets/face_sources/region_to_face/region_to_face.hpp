#ifndef MESH_TOOLS_SETS_FACE_SOURCES_REGION_TO_FACE_REGION_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_REGION_TO_FACE_REGION_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionToFace
// Description
//   A topoSetSource to select faces belonging to topological connected region
//   (that contains given point)
// SourceFiles
//   region_to_face.cpp
#include "topo_set_source.hpp"
#include "packed_bool_list.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
class regionToFace
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of set to use
    word setName_;
    //- Coordinate that is nearest/on connected region
    point nearPoint_;
  // Private Member Functions
    //- Walk edge-face-edge
    void markZone
    (
      const indirectPrimitivePatch& patch,
      const label procI,
      const label faceI,
      const label zoneI,
      labelList& faceZone
    ) const;
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("regionToFace");
  // Constructors
    //- Construct from components
    regionToFace
    (
      const polyMesh& mesh,
      const word& setName,
      const point& nearPoint
    );
    //- Construct from dictionary
    regionToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    regionToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~regionToFace();
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
