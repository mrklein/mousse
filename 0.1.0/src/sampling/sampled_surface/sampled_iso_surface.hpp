#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_ISO_SURFACE_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_ISO_SURFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledIsoSurface
// Description
//   A sampledSurface defined by a surface of iso value. Always triangulated.
//   To be used in sampleSurfaces / functionObjects. Recalculates iso surface
//   only if time changes.
// SourceFiles
//   sampled_iso_surface.cpp
#include "iso_surface.hpp"
#include "sampled_surface.hpp"
#include "zone_i_ds.hpp"
#include "fv_mesh_subset.hpp"
namespace mousse
{
class sampledIsoSurface
:
  public sampledSurface
{
  // Private data
    //- Field to get isoSurface of
    const word isoField_;
    //- Iso value
    const scalar isoVal_;
    //- Merge tolerance
    const scalar mergeTol_;
    //- Whether to coarse
    const Switch regularise_;
    //- Whether to recalculate cell values as average of point values
    const Switch average_;
    //- Zone name/index (if restricted to zones)
    mutable cellZoneID zoneID_;
    //- For zones: patch to put exposed faces into
    mutable word exposedPatchName_;
    mutable autoPtr<isoSurface> surfPtr_;
    //- Triangles converted to faceList
    mutable autoPtr<faceList> facesPtr_;
    // Recreated for every isoSurface
      //- Time at last call, also track if surface needs an update
      mutable label prevTimeIndex_;
      //- Cached volfield
      mutable autoPtr<volScalarField> storedVolFieldPtr_;
      mutable const volScalarField* volFieldPtr_;
      //- Cached pointfield
      mutable autoPtr<pointScalarField> storedPointFieldPtr_;
      mutable const pointScalarField* pointFieldPtr_;
      // And on subsetted mesh
        //- Cached submesh
        mutable autoPtr<fvMeshSubset> subMeshPtr_;
        //- Cached volfield
        mutable autoPtr<volScalarField> storedVolSubFieldPtr_;
        mutable const volScalarField* volSubFieldPtr_;
        //- Cached pointfield
        mutable autoPtr<pointScalarField> storedPointSubFieldPtr_;
        mutable const pointScalarField* pointSubFieldPtr_;
  // Private Member Functions
    //- Get fields needed to recreate iso surface.
    void getIsoFields() const;
    //- Create iso surface (if time has changed)
    //  Do nothing (and return false) if no update was needed
    bool updateGeometry() const;
    //- Sample field on faces
    template<class Type>
    tmp<Field<Type> > sampleField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vField
    ) const;
    template<class Type>
    tmp<Field<Type> >
    interpolateField(const interpolation<Type>&) const;
public:
  //- Runtime type information
  TYPE_NAME("sampledIsoSurface");
  // Constructors
    //- Construct from dictionary
    sampledIsoSurface
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledIsoSurface();
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
      if (facesPtr_.empty())
      {
        const triSurface& s = surface();
        facesPtr_.reset(new faceList(s.size()));
        FOR_ALL(s, i)
        {
          facesPtr_()[i] = s[i].triFaceFace();
        }
      }
      return facesPtr_;
    }
    const isoSurface& surface() const
    {
      return surfPtr_();
    }
    //- Lookup or read isoField. Sets volFieldPtr_ and pointFieldPtr_.
    void getIsoField();
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
#ifdef NoRepository
#   include "sampled_iso_surface_templates.cpp"
#endif
#endif
