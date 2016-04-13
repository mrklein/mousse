#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_CUTTING_PLANE_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_CUTTING_PLANE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledCuttingPlane
// Description
//   A sampledSurface defined by a plane

#include "sampled_surface.hpp"
#include "iso_surface.hpp"
#include "plane.hpp"
#include "zone_ids.hpp"
#include "fv_mesh_subset.hpp"


namespace mousse {

class sampledCuttingPlane
:
  public sampledSurface
{
  // Private data
    //- Plane
    const plane plane_;
    //- Merge tolerance
    const scalar mergeTol_;
    //- Whether to coarsen
    const Switch regularise_;
    //- Whether to recalculate cell values as average of point values
    const Switch average_;
    //- Zone name/index (if restricted to zones)
    mutable cellZoneID zoneID_;
    //- For zones: patch to put exposed faces into
    mutable word exposedPatchName_;
    //- Track if the surface needs an update
    mutable bool needsUpdate_;
    //- Optional subsetted mesh
    autoPtr<fvMeshSubset> subMeshPtr_;
    //- Distance to cell centres
    autoPtr<volScalarField> cellDistancePtr_;
    //- Distance to points
    scalarField pointDistance_;
    //- Constructed iso surface
    //autoPtr<isoSurfaceCell> isoSurfPtr_;
    autoPtr<isoSurface> isoSurfPtr_;
    //- Triangles converted to faceList
    mutable autoPtr<faceList> facesPtr_;
  // Private Member Functions
    //- Create iso surface
    void createGeometry();
    //- Sample field on faces
    template<class Type>
    tmp<Field<Type>> sampleField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vField
    ) const;
    template<class Type>
    tmp<Field<Type>>
    interpolateField(const interpolation<Type>&) const;
public:
  //- Runtime type information
  TYPE_NAME("sampledCuttingPlane");
  // Constructors
    //- Construct from dictionary
    sampledCuttingPlane
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledCuttingPlane();
  // Member Functions
    //- Does the surface need an update?
    virtual bool needsUpdate() const;
    //- Mark the surface as needing an update.
    //  May also free up unneeded data.
    //  Return false if surface was already marked as expired.
    virtual bool expire();
    //- Update the surface as required.
    //  Do nothing (and return false) if no update was needed
    virtual bool update();
    //- Points of surface
    virtual const pointField& points() const
    {
      return surface().points();
    }
    //- Faces of surface
    virtual const faceList& faces() const
    {
      if (facesPtr_.empty()) {
        const triSurface& s = surface();
        facesPtr_.reset(new faceList{s.size()});
        FOR_ALL(s, i) {
          facesPtr_()[i] = s[i].triFaceFace();
        }
      }
      return facesPtr_;
    }
    //const isoSurfaceCell& surface() const
    const isoSurface& surface() const
    {
      return isoSurfPtr_();
    }
    //- Sample field on surface
    virtual tmp<scalarField> sample
    (
      const volScalarField&
    ) const;
    //- Sample field on surface
    virtual tmp<vectorField> sample
    (
      const volVectorField&
    ) const;
    //- Sample field on surface
    virtual tmp<sphericalTensorField> sample
    (
      const volSphericalTensorField&
    ) const;
    //- Sample field on surface
    virtual tmp<symmTensorField> sample
    (
      const volSymmTensorField&
    ) const;
    //- Sample field on surface
    virtual tmp<tensorField> sample
    (
      const volTensorField&
    ) const;
    //- Interpolate field on surface
    virtual tmp<scalarField> interpolate
    (
      const interpolation<scalar>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<vectorField> interpolate
    (
      const interpolation<vector>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<sphericalTensorField> interpolate
    (
      const interpolation<sphericalTensor>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<symmTensorField> interpolate
    (
      const interpolation<symmTensor>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<tensorField> interpolate
    (
      const interpolation<tensor>&
    ) const;
    //- Write
    virtual void print(Ostream&) const;
};
}  // namespace mousse

#include "sampled_cutting_plane.ipp"

#endif
