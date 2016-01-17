// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::attachPolyTopoChanger
// Description
//   This class is derived from polyMesh and serves as a tool for
//   statically connecting pieces of a mesh by executing the mesh
//   modifiers and cleaning the mesh.
//   The idea is that a mesh can be built from pieces and put together
//   using various mesh modifiers (mainly sliding interfaces) which are
//   not needed during the run.  Therefore, once the mesh is assembled
//   and mesh modification triggered, the newly created point, face and
//   cell zones can be cleared together with the mesh modifiers thus
//   creating a singly connected static mesh.
//   Note:
//   All point, face and cell zoning will be lost!  Do it after
//   attaching the parts of the mesh, as the point, face and cell
//   numbering changes.
#ifndef attach_poly_topo_changer_hpp_
#define attach_poly_topo_changer_hpp_
#include "poly_topo_changer.hpp"
namespace mousse
{
class attachPolyTopoChanger
:
  public polyTopoChanger
{
public:
  // Constructors
    //- Read constructor from IOobject and a polyMesh
    attachPolyTopoChanger(const IOobject& io, polyMesh&);
    //- Read constructor for given polyMesh
    explicit attachPolyTopoChanger(polyMesh&);
    //- Disallow default bitwise copy construct
    attachPolyTopoChanger(const attachPolyTopoChanger&) = delete;
    //- Disallow default bitwise assignment
    attachPolyTopoChanger& operator=(const attachPolyTopoChanger&) = delete;
  //- Destructor
  virtual ~attachPolyTopoChanger()
  {}
  // Member Functions
    //- Attach mesh. By default filter out empty patches.
    void attach(const bool removeEmptyPatches = true);
};
}  // namespace mousse
#endif
