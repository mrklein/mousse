#ifndef FV_MOTION_SOLVER_FV_MOTION_SOLVERS_DISPLACEMENT_LAYERED_SOLVER_DISPLACEMENT_LAYERED_MOTION_MOTION_SOLVER_HPP_
#define FV_MOTION_SOLVER_FV_MOTION_SOLVERS_DISPLACEMENT_LAYERED_SOLVER_DISPLACEMENT_LAYERED_MOTION_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementLayeredMotionMotionSolver
// Description
//   Mesh motion solver for an (multi-block) extruded fvMesh. Gets given the
//   structure of the mesh blocks and boundary conditions on these blocks.
//   The displacementLayeredMotionCoeffs subdict of dynamicMeshDict specifies
//   per region (=cellZone) the boundary conditions on two opposing patches
//   (=faceZones). It then interpolates the boundary values using topological
//   walking so requires the cellZone to be a layered mesh.
//   The boundary conditions on faceZones are currently:
//   follow: the faceZone follows the overall mesh displacement.
//       Use this for faceZones on boundary faces (so it uses the
//       proper boundary conditions on the pointDisplacement).
//   uniformFollow: like 'follow' but takes the average value of
//       a specified 'patch' (which is not necessarily colocated)
//   fixedValue: fixed value.
//   timeVaryingUniformFixedValue: table-driven fixed value.
//   slip: the second of a pair of faceZones follows the tangential movement
//      specified by the first faceZone. (= removes the normal component).

#include "displacement_motion_solver.hpp"
#include "packed_bool_list.hpp"


namespace mousse {

// Forward class declarations
class displacementLayeredMotionMotionSolver
:
  public displacementMotionSolver
{
  // Private Member Functions
    void calcZoneMask
    (
      const label cellZoneI,
      PackedBoolList& isZonePoint,
      PackedBoolList& isZoneEdge
    ) const;
    void walkStructured
    (
      const label cellZoneI,
      const PackedBoolList& isZonePoint,
      const PackedBoolList& isZoneEdge,
      const labelList& seedPoints,
      const vectorField& seedData,
      scalarField& distance,
      vectorField& data
    ) const;
    tmp<vectorField> faceZoneEvaluate
    (
      const faceZone& fz,
      const labelList& meshPoints,
      const dictionary& dict,
      const PtrList<pointVectorField>& patchDisp,
      const label patchI
    ) const;
    void cellZoneSolve
    (
      const label cellZoneI,
      const dictionary& zoneDict
    );
public:
  //- Runtime type information
  TYPE_NAME("displacementLayeredMotion");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementLayeredMotionMotionSolver
    (
      const polyMesh&,
      const IOdictionary&
    );
    //- Disallow default bitwise copy construct
    displacementLayeredMotionMotionSolver
    (
      const displacementLayeredMotionMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    displacementLayeredMotionMotionSolver& operator=
    (
      const displacementLayeredMotionMotionSolver&
    ) = delete;
  //- Destructor
  ~displacementLayeredMotionMotionSolver();
  // Member Functions
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve();
    //- Update topology
    virtual void updateMesh(const mapPolyMesh&);
};

}  // namespace mousse

#endif

