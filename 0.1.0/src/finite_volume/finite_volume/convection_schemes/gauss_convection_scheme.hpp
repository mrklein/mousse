// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussConvectionScheme
// Description
//   Basic second-order convection using face-gradients and Gauss' theorem.
// SourceFiles
//   gauss_convection_scheme.cpp

#ifndef gauss_convection_scheme_hpp_
#define gauss_convection_scheme_hpp_

#include "convection_scheme.hpp"
#include "fv_mesh.hpp"

namespace mousse
{
namespace fv
{
//- Temporary debug switch to provide warning about backward-compatibility
//  issue with setting div schemes for steady-state
extern int warnUnboundedGauss;
template<class Type>
class gaussConvectionScheme
:
  public fv::convectionScheme<Type>
{
  // Private data
    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;
public:
  //- Runtime type information
  TYPE_NAME("Gauss");
  // Constructors
    //- Construct from flux and interpolation scheme
    gaussConvectionScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      const tmp<surfaceInterpolationScheme<Type> >& scheme
    )
    :
      convectionScheme<Type>{mesh, faceFlux},
      tinterpScheme_{scheme}
    {}
    //- Construct from flux and Istream
    gaussConvectionScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      convectionScheme<Type>{mesh, faceFlux},
      tinterpScheme_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      }
    {
      is.rewind();
      word bounded(is);
      if (warnUnboundedGauss
          && word(mesh.ddtScheme("default")) == "steadyState"
          && bounded != "bounded")
      {
        fileNameList controlDictFiles(findEtcFiles("controlDict"));
        IO_WARNING_IN("gaussConvectionScheme", is)
          << "Unbounded 'Gauss' div scheme used in "
           "steady-state solver, use 'bounded Gauss' "
           "to ensure boundedness.\n"
          << "    To remove this warning switch off "
          << "'warnUnboundedGauss' in "
          << controlDictFiles[controlDictFiles.size()-1]
          << endl;
      }
    }
    //- Disallow default bitwise copy construct
    gaussConvectionScheme(const gaussConvectionScheme&) = delete;
    //- Disallow default bitwise assignment
    gaussConvectionScheme& operator=(const gaussConvectionScheme&) = delete;
  // Member Functions
    const surfaceInterpolationScheme<Type>& interpScheme() const;
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<fvMatrix<Type> > fvmDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "gauss_convection_scheme.cpp"
#endif
#endif
