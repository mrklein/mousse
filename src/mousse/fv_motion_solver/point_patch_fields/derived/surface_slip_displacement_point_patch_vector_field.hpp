#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceSlipDisplacementPointPatchVectorField
// Description
//   Displacement follows a triSurface. Use in a displacementMotionSolver
//   as a bc on the pointDisplacement field.
//   Following is done by calculating the projection onto the surface according
//   to the projectMode
//   - NEAREST : nearest
//   - POINTNORMAL : intersection with point normal
//   - FIXEDNORMAL : intersection with fixed vector
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

#include "point_patch_fields.hpp"
#include "searchable_surfaces.hpp"


namespace mousse {

class surfaceSlipDisplacementPointPatchVectorField
:
  public pointPatchVectorField
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
  TYPE_NAME("surfaceSlipDisplacement");
  // Constructors
    //- Construct from patch and internal field
    surfaceSlipDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    surfaceSlipDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    surfaceSlipDisplacementPointPatchVectorField
    (
      const surfaceSlipDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    surfaceSlipDisplacementPointPatchVectorField
    (
      const surfaceSlipDisplacementPointPatchVectorField&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchVectorField> clone() const
    {
      return
        autoPtr<pointPatchVectorField>
        {
          new surfaceSlipDisplacementPointPatchVectorField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    surfaceSlipDisplacementPointPatchVectorField
    (
      const surfaceSlipDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchVectorField> clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return
        autoPtr<pointPatchVectorField>
        {
          new surfaceSlipDisplacementPointPatchVectorField{*this, iF}
        };
    }
    //- Disallow default bitwise assignment
    surfaceSlipDisplacementPointPatchVectorField& operator=
    (
      const surfaceSlipDisplacementPointPatchVectorField&
    ) = delete;
  // Member Functions
    //- Surface to follow. Demand loads surfaceNames.
    const searchableSurfaces& surfaces() const;
    //- Update the patch field
    virtual void evaluate
    (
      const Pstream::commsTypes commsType=Pstream::blocking
    );
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

