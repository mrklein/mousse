// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_semi_implicit_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "dimensioned_field.hpp"
#include "fvm_sup.hpp"
// Static Member Functions
template<class Type>
const mousse::wordList mousse::fv::SemiImplicitSource<Type>::volumeModeTypeNames_
(
  IStringStream("(absolute specific)")()
);
// Protected Member Functions 
template<class Type>
typename mousse::fv::SemiImplicitSource<Type>::volumeModeType
mousse::fv::SemiImplicitSource<Type>::wordToVolumeModeType
(
  const word& vmtName
) const
{
  FOR_ALL(volumeModeTypeNames_, i)
  {
    if (vmtName == volumeModeTypeNames_[i])
    {
      return volumeModeType(i);
    }
  }
  FATAL_ERROR_IN
  (
    "SemiImplicitSource<Type>::volumeModeType"
    "SemiImplicitSource<Type>::wordToVolumeModeType(const word&)"
  )   << "Unknown volumeMode type " << vmtName
    << ". Valid volumeMode types are:" << nl << volumeModeTypeNames_
    << exit(FatalError);
  return volumeModeType(0);
}
template<class Type>
mousse::word mousse::fv::SemiImplicitSource<Type>::volumeModeTypeToWord
(
  const volumeModeType& vmtType
) const
{
  if (vmtType > volumeModeTypeNames_.size())
  {
    return "UNKNOWN";
  }
  else
  {
    return volumeModeTypeNames_[vmtType];
  }
}
template<class Type>
void mousse::fv::SemiImplicitSource<Type>::setFieldData(const dictionary& dict)
{
  fieldNames_.setSize(dict.toc().size());
  injectionRate_.setSize(fieldNames_.size());
  applied_.setSize(fieldNames_.size(), false);
  label i = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter)
  {
    fieldNames_[i] = iter().keyword();
    dict.lookup(iter().keyword()) >> injectionRate_[i];
    i++;
  }
  // Set volume normalisation
  if (volumeMode_ == vmAbsolute)
  {
    VDash_ = V_;
  }
}
// Constructors 
template<class Type>
mousse::fv::SemiImplicitSource<Type>::SemiImplicitSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  volumeMode_(vmAbsolute),
  VDash_(1.0),
  injectionRate_()
{
  read(dict);
}
// Member Functions 
template<class Type>
void mousse::fv::SemiImplicitSource<Type>::addSup
(
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug)
  {
    Info<< "SemiImplicitSource<" << pTraits<Type>::typeName
      << ">::addSup for source " << name_ << endl;
  }
  const GeometricField<Type, fvPatchField, volMesh>& psi = eqn.psi();
  DimensionedField<Type, volMesh> Su
  (
    IOobject
    (
      name_ + fieldNames_[fieldI] + "Su",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensioned<Type>
    (
      "zero",
      eqn.dimensions()/dimVolume,
      pTraits<Type>::zero
    ),
    false
  );
  UIndirectList<Type>(Su, cells_) = injectionRate_[fieldI].first()/VDash_;
  DimensionedField<scalar, volMesh> Sp
  (
    IOobject
    (
      name_ + fieldNames_[fieldI] + "Sp",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensioned<scalar>
    (
      "zero",
      Su.dimensions()/psi.dimensions(),
      0.0
    ),
    false
  );
  UIndirectList<scalar>(Sp, cells_) = injectionRate_[fieldI].second()/VDash_;
  eqn += Su + fvm::SuSp(Sp, psi);
}
template<class Type>
void mousse::fv::SemiImplicitSource<Type>::addSup
(
  const volScalarField& /*rho*/,
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug)
  {
    Info<< "SemiImplicitSource<" << pTraits<Type>::typeName
      << ">::addSup for source " << name_ << endl;
  }
  return this->addSup(eqn, fieldI);
}
