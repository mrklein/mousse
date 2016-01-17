// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrude2DMesh
// Description
//   Given a 2D mesh insert all the topology changes to extrude. Does not work
//   in parallel.
// SourceFiles
//   extrude_2d_mesh.cpp
#ifndef extrude_2d_mesh_hpp_
#define extrude_2d_mesh_hpp_
#include "type_info.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "label_list.hpp"
#include "dictionary.hpp"
#include "extrude_model.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class mapPolyMesh;
class mapDistributePolyMesh;
class polyBoundaryMesh;
class extrude2DMesh
{
  // Private data
    //- Reference to 2D mesh
    polyMesh& mesh_;
    const dictionary dict_;
    //const dictionary patchDict_;
    const extrudeModel& model_;
    const word modelType_;
    const word patchType_;
    label frontPatchI_;
    label backPatchI_;
  // Private Member Functions
    //- Check the mesh is 2D
    void check2D() const;
    //- Find extrusion direction
    //void findExtrudeDirection();
public:
  //- Runtime type information
  CLASS_NAME("extrude2DMesh");
  // Constructors
    extrude2DMesh
    (
      polyMesh&,
      const dictionary& dict,
      const extrudeModel& model
    );
    //- Disallow default bitwise copy construct
    extrude2DMesh(const extrude2DMesh&) = delete;
    //- Disallow default bitwise assignment
    extrude2DMesh& operator=(const extrude2DMesh&) = delete;
  //- Destructor
  ~extrude2DMesh();
  // Member Functions
    //- Add front and back patches
    void addFrontBackPatches();
    //- Play commands into polyTopoChange to extrude mesh.
    void setRefinement(polyTopoChange&);
    //- Force recalculation of locally stored data on topological change
    void updateMesh(const mapPolyMesh&)
    {}
    //- Force recalculation of locally stored data for mesh distribution
    void distribute(const mapDistributePolyMesh&)
    {}
    label frontPatchI() const
    {
      return frontPatchI_;
    }
    label backPatchI() const
    {
      return backPatchI_;
    }
};
}  // namespace mousse
#endif
