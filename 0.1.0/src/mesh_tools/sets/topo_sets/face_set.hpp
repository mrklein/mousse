// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceSet
// Description
//   A list of face labels.
// SourceFiles
//   face_set.cpp
#ifndef face_set_hpp_
#define face_set_hpp_
#include "topo_set.hpp"
namespace mousse
{
class faceSet
:
  public topoSet
{
public:
  //- Runtime type information
  TypeName("faceSet");
  // Constructors
    //- Construct from IOobject
    faceSet(const IOobject& obj);
    //- Construct from objectRegistry and name
    faceSet
    (
      const polyMesh& mesh,
      const word& name,
      readOption r=MUST_READ,
      writeOption w=NO_WRITE
    );
    //- Construct from additional size of labelHashSet
    faceSet
    (
      const polyMesh& mesh,
      const word& name,
      const label,
      writeOption w=NO_WRITE
    );
    //- Construct from existing set
    faceSet
    (
      const polyMesh& mesh,
      const word& name,
      const topoSet&,
      writeOption w=NO_WRITE
    );
    //- Construct from additional labelHashSet
    faceSet
    (
      const polyMesh& mesh,
      const word& name,
      const labelHashSet&,
      writeOption w=NO_WRITE
    );
  //- Destructor
  virtual ~faceSet();
  // Member functions
    //- Sync faceSet across coupled patches.
    virtual void sync(const polyMesh& mesh);
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
