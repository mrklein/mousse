#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_EXTERNAL_DISPLACEMENT_MESH_MOVER_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_EXTERNAL_DISPLACEMENT_MESH_MOVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::externalDisplacementMeshMover
// Description
//   Virtual base class for mesh movers with externally provided displacement
//   field giving the boundary conditions. Move the mesh from the current
//   location to a new location (so modify the mesh; v.s. motionSolver that
//   only returns the new location).
//   All mesh movers are expected to read the dictionary settings at invocation
//   of move(), i.e. not cache any settings.

#include "point_fields.hpp"


namespace mousse {

class mapPolyMesh;


class externalDisplacementMeshMover
{
protected:
  // Protected data
    //- Baffles in the mesh
    List<labelPair> baffles_;
    //- Reference to point motion field
    pointVectorField& pointDisplacement_;
public:
  //- Runtime type information
  TYPE_NAME("externalDisplacementMeshMover");
  // Declare run-time New selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      externalDisplacementMeshMover,
      dictionary,
      (
        const dictionary& dict,
        const List<labelPair>& baffles,
        pointVectorField& pointDisplacement
      ),
      (dict, baffles, pointDisplacement)
    );
  // Constructors
    //- Construct from dictionary and displacement field. Dictionary is
    //  allowed to go out of scope!
    externalDisplacementMeshMover
    (
      const dictionary& dict,
      const List<labelPair>& baffles,
      pointVectorField& pointDisplacement
    );
    //- Disallow default bitwise copy construct
    externalDisplacementMeshMover
    (
      const externalDisplacementMeshMover&
    ) = delete;
    //- Disallow default bitwise assignment
    externalDisplacementMeshMover& operator=
    (
      const externalDisplacementMeshMover&
    ) = delete;
  // Selectors
    //- Return a reference to the selected meshMover model
    static autoPtr<externalDisplacementMeshMover> New
    (
      const word& type,
      const dictionary& dict,
      const List<labelPair>& baffles,
      pointVectorField& pointDisplacement
    );
  //- Destructor
  virtual ~externalDisplacementMeshMover();
  // Member Functions
    // Access
      //- Return reference to the point motion displacement field
      pointVectorField& pointDisplacement()
      {
        return pointDisplacement_;
      }
      //- Return const reference to the point motion displacement field
      const pointVectorField& pointDisplacement() const
      {
        return pointDisplacement_;
      }
      const pointMesh& pMesh() const
      {
        return pointDisplacement_.mesh();
      }
      const polyMesh& mesh() const
      {
        return pMesh()();
      }
    // Mesh mover
      //- Move mesh using current pointDisplacement boundary values
      //  and current dictionary settings. Return true if succesful
      //  (errors  on checkFaces less than allowable). Updates
      //  pointDisplacement.
      virtual bool move
      (
        const dictionary&,
        const label nAllowableErrors,
        labelList& checkFaces
      ) = 0;
      //- Update local data for geometry changes
      virtual void movePoints(const pointField&);
      //-  Update local data for topology changes
      virtual void updateMesh(const mapPolyMesh&);
};

}  // namespace mousse

#endif

