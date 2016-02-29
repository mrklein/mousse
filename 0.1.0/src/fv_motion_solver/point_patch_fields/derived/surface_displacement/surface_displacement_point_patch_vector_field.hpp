#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_SURFACE_DISPLACEMENT_SURFACE_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_SURFACE_DISPLACEMENT_SURFACE_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceDisplacementPointPatchVectorField
// Description
//   Displacement fixed by projection onto triSurface.
//   Use in a displacementMotionSolver
//   as a bc on the pointDisplacement field.
//   Calculates the projection onto the surface according
//   to the projectMode
//   - NEAREST : nearest
//   - POINTNORMAL : intersection with point normal
//   - FIXEDNORMAL : intersection with fixed vector
//   This displacement is then clipped with the specified velocity * deltaT.
//   Optionally (intersection only) removes a component ("wedgePlane") to
//   stay in 2D.
//   Needs:
//   - geometry : dictionary with searchableSurfaces. (usually
//    triSurfaceMeshes in constant/triSurface)
//   - projectMode : see above
//   - projectDirection : if projectMode = fixedNormal
//   - wedgePlane : -1 or component to knock out of intersection normal
//   - frozenPointsZone : empty or name of pointZone containing points
//             that do not move
// SourceFiles
//   surface_displacement_point_patch_vector_field.cpp
#include "point_patch_fields.hpp"
#include "fixed_value_point_patch_fields.hpp"
#include "searchable_surfaces.hpp"
#include "switch.hpp"
namespace mousse
{
class surfaceDisplacementPointPatchVectorField
:
  public fixedValuePointPatchVectorField
{
public:
  // Public data types
    enum projectMode
    {
      NEAREST,
      POINTNORMAL,
      FIXEDNORMAL
    };
private:
  // Private data
    //- Project mode names
    static const NamedEnum<projectMode, 3> projectModeNames_;
    //- Maximum velocity
    const vector velocity_;
    //- Names of surfaces
    const dictionary surfacesDict_;
    //- How to project/project onto surface
    const projectMode projectMode_;
    //- Direction to project
    const vector projectDir_;
    //- Plane for 2D wedge case or -1.
    const label wedgePlane_;
    //- pointZone with frozen points
    const word frozenPointsZone_;
    //- Demand driven: surface to project
    mutable autoPtr<searchableSurfaces> surfacesPtr_;
  // Private Member Functions
    //- Calculate displacement (w.r.t. points0()) to project onto surface
    void calcProjection(vectorField& displacement) const;
public:
  //- Runtime type information
  TYPE_NAME("surfaceDisplacement");
  // Constructors
    //- Construct from patch and internal field
    surfaceDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    surfaceDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    surfaceDisplacementPointPatchVectorField
    (
      const surfaceDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    surfaceDisplacementPointPatchVectorField
    (
      const surfaceDisplacementPointPatchVectorField&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchVectorField> clone() const
    {
      return autoPtr<pointPatchVectorField>
      (
        new surfaceDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    surfaceDisplacementPointPatchVectorField
    (
      const surfaceDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchVectorField> clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchVectorField>
      {
        new surfaceDisplacementPointPatchVectorField{*this, iF}
      };
    }
    //- Disallow default bitwise assignment
    surfaceDisplacementPointPatchVectorField& operator=
    (
      const surfaceDisplacementPointPatchVectorField&
    ) = delete;
  // Member Functions
    //- Surface to follow. Demand loads surfaceNames.
    const searchableSurfaces& surfaces() const;
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
