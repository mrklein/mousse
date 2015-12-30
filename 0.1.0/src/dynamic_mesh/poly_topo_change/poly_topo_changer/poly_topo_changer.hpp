// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyTopoChanger
// Description
//   List of mesh modifiers defining the mesh dynamics.
// SourceFiles
//   poly_topo_changer.cpp
#ifndef poly_topo_changer_hpp_
#define poly_topo_changer_hpp_
#include "reg_ioobject.hpp"
#include "ptr_list.hpp"
#include "poly_mesh_modifier.hpp"
namespace mousse
{
class polyMesh;
class mapPolyMesh;
class polyBoundaryMesh;
class polyTopoChanger
:
  public PtrList<polyMeshModifier>,
  public regIOobject
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    polyTopoChanger(const polyTopoChanger&);
    //- Disallow default bitwise assignment
    void operator=(const polyTopoChanger&);
    void readModifiers();
protected:
  // Protected data
    //- Reference to mesh
    polyMesh& mesh_;
public:
  //- Runtime type information
  TypeName("polyTopoChanger");
  // Constructors
    //- Read constructor given IOobject and a polyMesh
    polyTopoChanger(const IOobject&, polyMesh&);
    //- Read constructor for given polyMesh
    explicit polyTopoChanger(polyMesh&);
  //- Destructor
  virtual ~polyTopoChanger()
  {}
  // Member functions
    //- Return the mesh reference
    const polyMesh& mesh() const
    {
      return mesh_;
    }
    //- Return a list of patch types
    wordList types() const;
    //- Return a list of patch names
    wordList names() const;
    //- Is topology change required
    bool changeTopology() const;
    //- Return topology change request
    autoPtr<polyTopoChange> topoChangeRequest() const;
    //- Modify point motion
    void modifyMotionPoints(pointField&) const;
    autoPtr<mapPolyMesh> changeMesh
    (
      const bool inflate,
      const bool syncParallel = true,
      const bool orderCells = false,
      const bool orderPoints = false
    );
    //- Force recalculation of locally stored data on topological change
    void update(const mapPolyMesh& m);
    //- Add given set of topology modifiers to the topoChanger
    void addTopologyModifiers(const List<polyMeshModifier*>& tm);
    //- Find modifier given a name
    label findModifierID(const word& modName) const;
    //- writeData member function required by regIOobject
    bool writeData(Ostream&) const;
  // Member Operators
    bool operator!=(const polyTopoChanger&) const;
    bool operator==(const polyTopoChanger&) const;
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const polyTopoChanger&);
};
}  // namespace mousse
#endif
