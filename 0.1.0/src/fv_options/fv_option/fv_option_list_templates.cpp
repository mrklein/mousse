// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  GeometricField<Type, fvPatchField, volMesh>& field
)
{
  return this->operator()(field, field.name());
}
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  GeometricField<Type, fvPatchField, volMesh>& field,
  const word& fieldName
)
{
  checkApplied();
  const dimensionSet ds = field.dimensions()/dimTime*dimVolume;
  tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(field, ds));
  fvMatrix<Type>& mtx = tmtx();
  forAll(*this, i)
  {
    option& source = this->operator[](i);
    label fieldI = source.applyToField(fieldName);
    if (fieldI != -1)
    {
      source.setApplied(fieldI);
      if (source.isActive())
      {
        if (debug)
        {
          Info<< "Applying source " << source.name() << " to field "
            << fieldName << endl;
        }
        source.addSup(mtx, fieldI);
      }
    }
  }
  return tmtx;
}
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  const volScalarField& rho,
  GeometricField<Type, fvPatchField, volMesh>& field
)
{
  return this->operator()(rho, field, field.name());
}
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  const volScalarField& rho,
  GeometricField<Type, fvPatchField, volMesh>& field,
  const word& fieldName
)
{
  checkApplied();
  const dimensionSet ds
  (
    rho.dimensions()*field.dimensions()/dimTime*dimVolume
  );
  tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(field, ds));
  fvMatrix<Type>& mtx = tmtx();
  forAll(*this, i)
  {
    option& source = this->operator[](i);
    label fieldI = source.applyToField(fieldName);
    if (fieldI != -1)
    {
      source.setApplied(fieldI);
      if (source.isActive())
      {
        if (debug)
        {
          Info<< "Applying source " << source.name() << " to field "
            << fieldName << endl;
        }
        source.addSup(rho, mtx, fieldI);
      }
    }
  }
  return tmtx;
}
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  const volScalarField& alpha,
  const volScalarField& rho,
  GeometricField<Type, fvPatchField, volMesh>& field
)
{
  return this->operator()(alpha, rho, field, field.name());
}
template<class Type>
mousse::tmp<mousse::fvMatrix<Type> > mousse::fv::optionList::operator()
(
  const volScalarField& alpha,
  const volScalarField& rho,
  GeometricField<Type, fvPatchField, volMesh>& field,
  const word& fieldName
)
{
  checkApplied();
  const dimensionSet ds
  (
    alpha.dimensions()*rho.dimensions()*field.dimensions()
   /dimTime*dimVolume
  );
  tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(field, ds));
  fvMatrix<Type>& mtx = tmtx();
  forAll(*this, i)
  {
    option& source = this->operator[](i);
    label fieldI = source.applyToField(fieldName);
    if (fieldI != -1)
    {
      source.setApplied(fieldI);
      if (source.isActive())
      {
        if (debug)
        {
          Info<< "Applying source " << source.name() << " to field "
            << fieldName << endl;
        }
        source.addSup(alpha, rho, mtx, fieldI);
      }
    }
  }
  return tmtx;
}
template<class Type>
void mousse::fv::optionList::constrain(fvMatrix<Type>& eqn)
{
  checkApplied();
  forAll(*this, i)
  {
    option& source = this->operator[](i);
    label fieldI = source.applyToField(eqn.psi().name());
    if (fieldI != -1)
    {
      source.setApplied(fieldI);
      if (source.isActive())
      {
        if (debug)
        {
          Info<< "Applying constraint " << source.name()
            << " to field " << eqn.psi().name() << endl;
        }
        source.constrain(eqn, fieldI);
      }
    }
  }
}
template<class Type>
void mousse::fv::optionList::correct
(
  GeometricField<Type, fvPatchField, volMesh>& field
)
{
  const word& fieldName = field.name();
  forAll(*this, i)
  {
    option& source = this->operator[](i);
    label fieldI = source.applyToField(fieldName);
    if (fieldI != -1)
    {
      source.setApplied(fieldI);
      if (source.isActive())
      {
        if (debug)
        {
          Info<< "Correcting source " << source.name()
            << " for field " << fieldName << endl;
        }
        source.correct(field);
      }
    }
  }
}
