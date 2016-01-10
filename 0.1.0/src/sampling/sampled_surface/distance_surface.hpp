// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::distanceSurface
// Description
//   A sampledSurface defined by a distance to a surface.
//   Uses either isoSurfaceCell or isoSurface.
// SourceFiles
//   distance_surface.cpp
#ifndef distance_surface_hpp_
#define distance_surface_hpp_
#include "sampled_surface.hpp"
#include "searchable_surface.hpp"
#include "iso_surface_cell.hpp"
#include "iso_surface.hpp"
namespace mousse
{
class distanceSurface
:
  public sampledSurface
{
  // Private data
    //- Surface
    const autoPtr<searchableSurface> surfPtr_;
    //- Distance value
    const scalar distance_;
    //- Signed distance
    const bool signed_;
    //- Whether to use isoSurfaceCell or isoSurface
    const bool cell_;
    //- Whether to coarsen
    const Switch regularise_;
    //- Whether to recalculate cell values as average of point values
    const Switch average_;
    //- If restricted to zones, name of this zone or a regular expression
    keyType zoneKey_;
    //- Track if the surface needs an update
    mutable bool needsUpdate_;
    //- Distance to cell centres
    autoPtr<volScalarField> cellDistancePtr_;
    //- Distance to points
    scalarField pointDistance_;
    //- Constructed iso surface
    autoPtr<isoSurfaceCell> isoSurfCellPtr_;
    //- Constructed iso surface
    autoPtr<isoSurface> isoSurfPtr_;
    //- Triangles converted to faceList
    mutable autoPtr<faceList> facesPtr_;
  // Private Member Functions
    //- Create iso surface
    void createGeometry();
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
  TYPE_NAME("distanceSurface");
  // Constructors
    //- Construct from dictionary
    distanceSurface
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from components
    distanceSurface
    (
      const word& name,
      const polyMesh& mesh,
      const bool interpolate,
      const word& surfaceType,
      const word& surfaceName,
      const scalar distance,
      const bool signedDistance,
      const bool cell,
      const Switch regularise,
      const Switch average
    );
  //- Destructor
  virtual ~distanceSurface();
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
    const triSurface& surface() const
    {
      if (cell_)
      {
        return isoSurfCellPtr_();
      }
      else
      {
        return isoSurfPtr_();
      }
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
#   include "distance_surface_templates.cpp"
#endif
#endif
