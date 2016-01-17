// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSet
// Description
//   A collection of cell labels.
// SourceFiles
//   cell_set.cpp

#ifndef cell_set_hpp_
#define cell_set_hpp_

#include "topo_set.hpp"
#include "type_info.hpp"

namespace mousse
{
class cellSet
:
  public topoSet
{
public:
  //- Runtime type information
  TYPE_NAME("cellSet");
  // Constructors
    //- Construct from IOobject. No checking.
    cellSet(const IOobject& obj);
    //- Construct from polyMesh and name. Checks for valid cell ids.
    cellSet
    (
      const polyMesh& mesh,
      const word& name,
      readOption r=MUST_READ,
      writeOption w=NO_WRITE
    );
    //- Construct empty from size of labelHashSet
    cellSet
    (
      const polyMesh& mesh,
      const word& name,
      const label sizes,
      writeOption w=NO_WRITE
    );
    //- Construct from existing set
    cellSet
    (
      const polyMesh& mesh,
      const word& name,
      const topoSet&,
      writeOption w=NO_WRITE
    );
    //- Construct from labelHashSet
    cellSet
    (
      const polyMesh& mesh,
      const word& name,
      const labelHashSet&,
      writeOption w=NO_WRITE
    );
    // Used for tetMesh cellSet only.
      //- Construct from objectRegistry and name.
      cellSet
      (
        const Time&,
        const word& name,
        readOption r=MUST_READ,
        writeOption w=NO_WRITE
      );
      //- Construct empty from objectRegistry.
      cellSet
      (
        const Time&,
        const word& name,
        const label size,
        writeOption w=NO_WRITE
      );
      //- Construct from labelHashSet
      cellSet
      (
        const Time&,
        const word& name,
        const labelHashSet&,
        writeOption w=NO_WRITE
      );
    //- Disallow default bitwise copy construct
    cellSet(const cellSet&) = delete;
  //- Destructor
  virtual ~cellSet();
  // Member functions
    //- Sync cellSet across coupled patches.
    virtual void sync(const polyMesh&)
    {}
    //- Return max index+1.
    virtual label maxSize(const polyMesh& mesh) const;
    //- Update any stored data for new labels
    virtual void updateMesh(const mapPolyMesh& morphMap);
    //- Write maxLen items with label and coordinates.
    virtual void writeDebug
    (
      Ostream& os,
      const primitiveMesh&,
      const label maxLen
    ) const;
};
}  // namespace mousse
#endif
