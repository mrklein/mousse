// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitFuncs::LimitFuncs
// Description
//   Class to create NVD/TVD limited weighting-factors.
//   The particular differencing scheme class is supplied as a template
//   argument, the weight function of which is called by the weight function
//   of this class for the internal faces as well as faces of coupled
//   patches (e.g. processor-processor patches). The weight function is
//   supplied the central-differencing weighting factor, the face-flux, the
//   cell and face gradients (from which the normalised variable
//   distribution may be created) and the cell centre distance.
//   This code organisation is both neat and efficient, allowing for
//   convenient implementation of new schemes to run on parallelised cases.
// SourceFiles
//   limit_funcs.cpp
#ifndef limit_funcs_hpp_
#define limit_funcs_hpp_
namespace mousse
{
namespace limitFuncs
{
template<class Type>
class null
{
public:
  null()
  {}
  inline tmp<GeometricField<Type, fvPatchField, volMesh> > operator()
  (
    const GeometricField<Type, fvPatchField, volMesh>& phi
  ) const
  {
    return phi;
  }
};
template<class Type>
class magSqr
{
public:
  magSqr()
  {}
  inline tmp<volScalarField> operator()
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  ) const;
};
template<class Type>
class rhoMagSqr
{
public:
  rhoMagSqr()
  {}
  inline tmp<volScalarField> operator()
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  ) const;
};
}  // namespace limitFuncs
}  // namespace mousse
#ifdef NoRepository
#   include "limit_funcs.cpp"
#endif
#endif
