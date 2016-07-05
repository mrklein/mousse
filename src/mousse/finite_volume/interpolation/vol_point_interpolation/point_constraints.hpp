#ifndef FINITE_VOLUME_INTERPOLATION_VOL_POINT_INTERPOLATION_POINT_CONSTRAINTS_HPP_
#define FINITE_VOLUME_INTERPOLATION_VOL_POINT_INTERPOLATION_POINT_CONSTRAINTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointConstraints
// Description
//   Application of (multi-)patch point contraints.
//   Note: includes all points which are on the boundary of a patch
//      with a constraint. It includes them (even though the constraint
//      will already be implemented through the patch evaluation)
//      since these points might be
//      coupled to points which are not on any constraint patch and we
//      don't want to get inconsistency between the two points.

#include "_mesh_object.hpp"
#include "tensor_field.hpp"
#include "point_fields_fwd.hpp"
#include "point_constraint.hpp"


namespace mousse {

class pointMesh;
class polyMesh;


class pointConstraints
:
  public MeshObject<pointMesh, UpdateableMeshObject, pointConstraints>
{
  // Private data
    // Patch-patch constraints
      //- Mesh points on which to apply special constraints
      labelList patchPatchPointConstraintPoints_;
      //- Special constraints (as tensors)
      tensorField patchPatchPointConstraintTensors_;
      //- Special constraints (raw)
      List<pointConstraint> patchPatchPointConstraints_;
  // Private Member Functions
    //- Make patch-patch constraints
    void makePatchPatchAddressing();
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("pointConstraints");
  // Constructors
    //- Constructor from pointMesh.
    explicit pointConstraints(const pointMesh&);
    //- Disallow default bitwise copy construct
    pointConstraints(const pointConstraints&) = delete;
    //- Disallow default bitwise assignment
    pointConstraints& operator=(const pointConstraints&) = delete;
  //- Destructor
  ~pointConstraints();
  // Member functions
    // Access
      //- Mesh points on which to apply special constraints
      const labelList& patchPatchPointConstraintPoints() const
      {
        return patchPatchPointConstraintPoints_;
      }
      //- Special constraints
      const tensorField& patchPatchPointConstraintTensors() const
      {
        return patchPatchPointConstraintTensors_;
      }
      //- Actual constraints
      const List<pointConstraint>& patchPatchPointConstraints() const
      {
        return patchPatchPointConstraints_;
      }
    // Edit
      //- Update mesh topology using the morph engine
      void updateMesh(const mapPolyMesh&);
      //- Correct weighting factors for moving mesh.
      bool movePoints();
    // Interpolation functions
      //- Helper: sync data on collocated points only
      template<class Type, class CombineOp>
      static void syncUntransformedData
      (
        const polyMesh& mesh,
        List<Type>& pointData,
        const CombineOp& cop
      );
      //- Helper: set patchField values from internal values (on
      //  valuePointPatchFields). Opposite of
      //  pointPatchField::setInInternalField
      template<class Type>
      static void setPatchFields
      (
        GeometricField<Type, pointPatchField, pointMesh>&
      );
      //- Apply patch-patch constraints only
      template<class Type>
      void constrainCorners
      (
        GeometricField<Type, pointPatchField, pointMesh>& pf
      ) const;
      //- Apply boundary conditions (single-patch constraints) and
      //  patch-patch constraints on generic pointField.
      //  Optionally override valuePointPatchFields with constrained
      //  values (see setPatchFields above)
      template<class Type>
      void constrain
      (
        GeometricField<Type, pointPatchField, pointMesh>& pf,
        const bool overrideValue = false
      ) const;
      //- Apply boundary conditions (single-patch constraints),
      //  patch-patch constraints and
      //  two-D constraints on displacement field
      void constrainDisplacement
      (
        pointVectorField& displacement,
        const bool overrideValue = false
      ) const;
};

template<>
void pointConstraints::constrainCorners<scalar>
(
  GeometricField<scalar, pointPatchField, pointMesh>& pf
) const;

template<>
void pointConstraints::constrainCorners<label>
(
  GeometricField<label, pointPatchField, pointMesh>& pf
) const;

}  // namespace mousse


#include "point_constraints.ipp"

#endif
