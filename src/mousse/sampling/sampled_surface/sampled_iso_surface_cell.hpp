#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_ISO_SURFACE_CELL_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_ISO_SURFACE_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledIsoSurfaceCell
// Description
//   A sampledSurface defined by a surface of iso value. Always triangulated.
//   To be used in sampleSurfaces / functionObjects. Recalculates iso surface
//   only if time changes.

#include "sampled_surface.hpp"
#include "tri_surface.hpp"


namespace mousse {

class sampledIsoSurfaceCell
:
  public sampledSurface,
  public triSurface
{
  // Private data
    //- Field to get isoSurface of
    const word isoField_;
    //- Iso value
    const scalar isoVal_;
    //- Whether to coarse
    const Switch regularise_;
    //- Whether to recalculate cell values as average of point values
    const Switch average_;
    //- If restricted to zones, name of this zone or a regular expression
    keyType zoneKey_;
    //- Triangles converted to faceList
    mutable autoPtr<faceList> facesPtr_;
    // Recreated for every isoSurface
      //- Time at last call, also track it surface needs an update
      mutable label prevTimeIndex_;
      //- For every triangle the original cell in mesh
      mutable labelList meshCells_;
  // Private Member Functions
    //- Create iso surface (if time has changed)
    //  Do nothing (and return false) if no update was needed
    bool updateGeometry() const;
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
  TYPE_NAME("sampledIsoSurfaceCell");
  // Constructors
    //- Construct from dictionary
    sampledIsoSurfaceCell
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledIsoSurfaceCell();
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
      return triSurface::points();
    }
    //- Faces of surface
    virtual const faceList& faces() const
    {
      if (facesPtr_.empty()) {
        const triSurface& s = *this;
        facesPtr_.reset(new faceList{s.size()});
        FOR_ALL(s, i) {
          facesPtr_()[i] = s[i].triFaceFace();
        }
      }
      return facesPtr_;
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

#include "sampled_iso_surface_cell.ipp"

#endif
