#ifndef MESH_TOOLS_SETS_TOPO_SETS_POINT_SET_HPP_
#define MESH_TOOLS_SETS_TOPO_SETS_POINT_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointSet
// Description
//   A set of point labels.

#include "topo_set.hpp"


namespace mousse {

class pointSet
:
  public topoSet
{
public:
  //- Runtime type information
  TYPE_NAME("pointSet");
  // Constructors
    //- Construct from IOobject
    pointSet(const IOobject& obj);
    //- Construct from objectRegistry and name
    pointSet
    (
      const polyMesh& mesh,
      const word& name,
      readOption r=MUST_READ,
      writeOption w=NO_WRITE
    );
    //- Construct from additional size of labelHashSet
    pointSet
    (
      const polyMesh& mesh,
      const word& name,
      const label,
      writeOption w=NO_WRITE
    );
    //- Construct from additional labelHashSet
    pointSet
    (
      const polyMesh& mesh,
      const word& name,
      const topoSet&,
      writeOption w=NO_WRITE
    );
    //- Construct from additional labelHashSet
    pointSet
    (
      const polyMesh& mesh,
      const word& name,
      const labelHashSet&,
      writeOption w=NO_WRITE
    );
  //- Destructor
  virtual ~pointSet();
  // Member functions
    //- Sync set across coupled patches. Adds coupled points to set.
    virtual void sync(const polyMesh& mesh);
    //- Return max index+1.
    virtual label maxSize(const polyMesh& mesh) const;
    //- Update any stored data for new labels
    virtual void updateMesh(const mapPolyMesh& morphMap);
    //- Update any stored data for new labels
    //virtual void updateMesh(const polyTopoChange& meshMod);
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
