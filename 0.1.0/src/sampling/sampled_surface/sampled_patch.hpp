// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledPatch
// Description
//   A sampledSurface on patches. Non-triangulated by default.
// SourceFiles
//   sampled_patch.cpp
#ifndef sampled_patch_hpp_
#define sampled_patch_hpp_
#include "sampled_surface.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
class sampledPatch
:
  public MeshedSurface<face>,
  public sampledSurface
{
  //- Private typedefs for convenience
  typedef MeshedSurface<face> MeshStorage;
  // Private data
    //- Name of patches
    const wordReList patchNames_;
    //- Corresponding patchIDs
    mutable labelList patchIDs_;
    //- Triangulated faces or keep faces as is
    bool triangulate_;
    //- Track if the surface needs an update
    mutable bool needsUpdate_;
    //- For every face (or triangle) the originating patch
    labelList patchIndex_;
    //- For every face (or triangle) the index in the originating patch
    labelList patchFaceLabels_;
    //- Start indices (in patchFaceLabels_) of patches
    labelList patchStart_;
  // Private Member Functions
    //- Sample field on faces
    template<class Type>
    tmp<Field<Type> > sampleField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vField
    ) const;
    //- Sample surface field on faces
    template<class Type>
    tmp<Field<Type> > sampleField
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
    ) const;
    template<class Type>
    tmp<Field<Type> > interpolateField(const interpolation<Type>&) const;
    //- Re-map action on triangulation or cleanup
    virtual void remapFaces(const labelUList& faceMap);
protected:
    const wordReList& patchNames() const
    {
      return patchNames_;
    }
    const labelList& patchIDs() const;
    const labelList& patchStart() const
    {
      return patchStart_;
    }
    const labelList& patchFaceLabels() const
    {
      return patchFaceLabels_;
    }
public:
  //- Runtime type information
  TYPE_NAME("sampledPatch");
  // Constructors
    //- Construct from components
    sampledPatch
    (
      const word& name,
      const polyMesh& mesh,
      const wordReList& patchNames,
      const bool triangulate = false
    );
    //- Construct from dictionary
    sampledPatch
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledPatch();
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
    // Sample
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
      //- Surface sample field on surface
      virtual tmp<scalarField> sample
      (
        const surfaceScalarField&
      ) const;
      //- Surface Sample field on surface
      virtual tmp<vectorField> sample
      (
        const surfaceVectorField&
      ) const;
      //- Surface sample field on surface
      virtual tmp<sphericalTensorField> sample
      (
        const surfaceSphericalTensorField&
      ) const;
      //- Surface sample field on surface
      virtual tmp<symmTensorField> sample
      (
        const surfaceSymmTensorField&
      ) const;
      //- Surface sample field on surface
      virtual tmp<tensorField> sample
      (
        const surfaceTensorField&
      ) const;
    // Interpolate
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
#   include "sampled_patch_templates.cpp"
#endif
#endif
