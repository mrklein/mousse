#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_THRESHOLD_CELL_FACES_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_THRESHOLD_CELL_FACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledThresholdCellFaces
// Description
//   A sampledSurface defined by the cell faces corresponding to a threshold
//   value.
// SourceFiles
//   sampled_threshold_cell_faces.cpp
#include "sampled_surface.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
class sampledThresholdCellFaces
:
  public sampledSurface,
  public MeshedSurface<face>
{
  //- Private typedefs for convenience
    typedef MeshedSurface<face> MeshStorage;
  // Private data
    //- Field to get isoSurface of
    const word fieldName_;
    //- Threshold value
    const scalar lowerThreshold_;
    //- Threshold value
    const scalar upperThreshold_;
    //- If restricted to zones, name of this zone or a regular expression
    keyType zoneKey_;
    //- Triangulated faces or keep faces as is
    bool triangulate_;
    // Recreated for every time-step
    //- Time at last call, also track it surface needs an update
    mutable label prevTimeIndex_;
    //- For every face the original cell in mesh
    mutable labelList meshCells_;
  // Private Member Functions
    //- Create surface (if time has changed)
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
  TYPE_NAME("sampledThresholdCellFaces");
  // Constructors
    //- Construct from dictionary
    sampledThresholdCellFaces
    (
      const word& name,
      const polyMesh&,
      const dictionary&
    );
  //- Destructor
  virtual ~sampledThresholdCellFaces();
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
      return MeshStorage::points();
    }
    //- Faces of surface
    virtual const faceList& faces() const
    {
      return MeshStorage::faces();
    }
    //- Sample field on surface
    virtual tmp<scalarField> sample(const volScalarField&) const;
    //- Sample field on surface
    virtual tmp<vectorField> sample( const volVectorField&) const;
    //- Sample field on surface
    virtual tmp<sphericalTensorField> sample
    (
      const volSphericalTensorField&
    ) const;
    //- Sample field on surface
    virtual tmp<symmTensorField> sample(const volSymmTensorField&) const;
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
#   include "sampled_threshold_cell_faces_templates.cpp"
#endif
#endif
