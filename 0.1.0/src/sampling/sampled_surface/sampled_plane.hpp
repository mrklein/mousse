// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledPlane
// Description
//   A sampledSurface defined by a cuttingPlane. Triangulated by default.
// Note
//   Does not actually cut until update() called.
// SourceFiles
//   sampled_plane.cpp
#ifndef sampled_plane_hpp_
#define sampled_plane_hpp_
#include "sampled_surface.hpp"
#include "cutting_plane.hpp"
namespace mousse
{
class sampledPlane
:
  public sampledSurface,
  public cuttingPlane
{
  // Private data
    //- If restricted to zones, name of this zone or a regular expression
    keyType zoneKey_;
    //- Triangulated faces or keep faces as is
    const bool triangulate_;
    //- Track if the surface needs an update
    mutable bool needsUpdate_;
  // Private Member Functions
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
  TypeName("sampledPlane");
  // Constructors
    //- Construct from components
    sampledPlane
    (
      const word& name,
      const polyMesh& mesh,
      const plane& planeDesc,
      const keyType& zoneKey = word::null,
      const bool triangulate = true
    );
    //- Construct from dictionary
    sampledPlane
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledPlane();
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
      return cuttingPlane::points();
    }
    //- Faces of surface
    virtual const faceList& faces() const
    {
      return cuttingPlane::faces();
    }
    //- For every face original cell in mesh
    const labelList& meshCells() const
    {
      return cuttingPlane::cutCells();
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
#ifdef NoRepository
#   include "sampled_plane_templates.cpp"
#endif
#endif
