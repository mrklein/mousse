#ifndef MESH_TOOLS_SETS_TOPO_SETS_FACE_ZONE_SET_HPP_
#define MESH_TOOLS_SETS_TOPO_SETS_FACE_ZONE_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceZoneSet
// Description
//   Like faceSet but updates faceZone when writing.
// SourceFiles
//   face_zone.cpp
#include "face_set.hpp"
#include "bool_list.hpp"
namespace mousse
{
class faceZoneSet
:
  public faceSet
{
  // Private data
    const polyMesh& mesh_;
    labelList addressing_;
    boolList flipMap_;
 // Private Member Functions
public:
  //- Runtime type information
  TYPE_NAME("faceZoneSet");
  // Constructors
    //- Construct from objectRegistry and name
    faceZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      readOption r=MUST_READ,
      writeOption w=NO_WRITE
    );
    //- Construct from additional size of labelHashSet
    faceZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      const label,
      writeOption w=NO_WRITE
    );
    //- Construct from existing set
    faceZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      const topoSet&,
      writeOption w=NO_WRITE
    );
  //- Destructor
  virtual ~faceZoneSet();
  // Member functions
    const labelList& addressing() const
    {
      return addressing_;
    }
    labelList& addressing()
    {
      return addressing_;
    }
    const boolList& flipMap() const
    {
      return flipMap_;
    }
    boolList& flipMap()
    {
      return flipMap_;
    }
    //- Sort addressing and make faceSet part consistent with addressing
    void updateSet();
    //- Invert contents. (insert all members 0..maxLen-1 which were not in
    //  set)
    virtual void invert(const label maxLen);
    //- Subset contents. Only elements present in both sets remain.
    virtual void subset(const topoSet& set);
    //- Add elements present in set.
    virtual void addSet(const topoSet& set);
    //- Delete elements present in set.
    virtual void deleteSet(const topoSet& set);
    //- Sync faceZoneSet across coupled patches.
    virtual void sync(const polyMesh& mesh);
    //- Write maxLen items with label and coordinates.
    virtual void writeDebug
    (
      Ostream& os,
      const primitiveMesh&,
      const label maxLen
    ) const;
    //- Write faceZone
    virtual bool writeObject
    (
      IOstream::streamFormat,
      IOstream::versionNumber,
      IOstream::compressionType
    ) const;
    //- Update any stored data for new labels
    virtual void updateMesh(const mapPolyMesh& morphMap);
    //- Return max index+1.
    virtual label maxSize(const polyMesh& mesh) const;
};
}  // namespace mousse
#endif
