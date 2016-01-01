// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mergePolyMesh
// Description
//   Add a given mesh to the original mesh to create a single new mesh
// SourceFiles
//   merge_poly_mesh.cpp
#ifndef merge_poly_mesh_hpp_
#define merge_poly_mesh_hpp_
#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"
namespace mousse
{
// Forward declaration of classes
class mergePolyMesh
:
  public polyMesh
{
  // Private data
    //- Topological change to accumulated all mesh changes
    polyTopoChange meshMod_;
    //- Patch names
    DynamicList<word> patchNames_;
    //- Patch dictionaries
    DynamicList<dictionary> patchDicts_;
    //- Point zone names
    DynamicList<word> pointZoneNames_;
    //- Face zone names
    DynamicList<word> faceZoneNames_;
    //- Cell zone names
    DynamicList<word> cellZoneNames_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    mergePolyMesh(const mergePolyMesh&);
    //- Disallow default bitwise assignment
    void operator=(const mergePolyMesh&);
    //- Return patch index given a name and type
    label patchIndex(const polyPatch&);
    //- Return zone index given a list of active zones and a name
    label zoneIndex(DynamicList<word>&, const word&);
public:
  //- Runtime type information
  TypeName("mergePolyMesh");
  // Constructors
    //- Construct from IOobject
    mergePolyMesh(const IOobject& io);
  //- Destructor
  virtual ~mergePolyMesh()
  {}
  // Member Functions
    //- Add a mesh
    void addMesh(const polyMesh& m);
    //- Merge meshes
    void merge();
};
}  // namespace mousse
#endif
