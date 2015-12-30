// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellZoneSet
// Description
//   Like cellSet but updates cellZone when writing.
// SourceFiles
//   cell_zone.cpp
#ifndef cell_zone_set_hpp_
#define cell_zone_set_hpp_
#include "cell_set.hpp"
#include "bool_list.hpp"
namespace mousse
{
class cellZoneSet
:
  public cellSet
{
  // Private data
    const polyMesh& mesh_;
    labelList addressing_;
public:
  //- Runtime type information
  TypeName("cellZoneSet");
  // Constructors
    //- Construct from objectRegistry and name
    cellZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      readOption r=MUST_READ,
      writeOption w=NO_WRITE
    );
    //- Construct from additional size of labelHashSet
    cellZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      const label,
      writeOption w=NO_WRITE
    );
    //- Construct from existing set
    cellZoneSet
    (
      const polyMesh& mesh,
      const word& name,
      const topoSet&,
      writeOption w=NO_WRITE
    );
  //- Destructor
  virtual ~cellZoneSet();
  // Member functions
    const labelList& addressing() const
    {
      return addressing_;
    }
    labelList& addressing()
    {
      return addressing_;
    }
    //- Sort addressing and make cellSet part consistent with addressing
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
    //- Sync cellZoneSet across coupled patches.
    virtual void sync(const polyMesh& mesh);
    //- Write maxLen items with label and coordinates.
    virtual void writeDebug
    (
      Ostream& os,
      const primitiveMesh&,
      const label maxLen
    ) const;
    //- Write cellZone
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
