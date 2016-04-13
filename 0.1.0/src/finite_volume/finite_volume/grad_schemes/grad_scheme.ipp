// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv.hpp"
#include "object_registry.hpp"
#include "solution.hpp"


// Selectors
template<class Type>
mousse::tmp<mousse::fv::gradScheme<Type> > mousse::fv::gradScheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (fv::debug) {
    Info << "gradScheme<Type>::New"
       "(const fvMesh& mesh, Istream& schemeData) : "
       "constructing gradScheme<Type>"
      << endl;
  }

  if (schemeData.eof()) {
    FATAL_IO_ERROR_IN
    (
      "gradScheme<Type>::New"
      "(const fvMesh& mesh, Istream& schemeData)",
      schemeData
    )
    << "Grad scheme not specified" << endl << endl
    << "Valid grad schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }

  const word schemeName{schemeData};
  typename IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(schemeName);

  if (cstrIter == IstreamConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "gradScheme<Type>::New"
      "(const fvMesh& mesh, Istream& schemeData)",
      schemeData
    )
    << "Unknown grad scheme " << schemeName << nl << nl
    << "Valid grad schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }

  return cstrIter()(mesh, schemeData);
}


// Destructor 
template<class Type>
mousse::fv::gradScheme<Type>::~gradScheme()
{}


template<class Type>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<mousse::vector, Type>::type,
    mousse::fvPatchField,
    mousse::volMesh
  >
>
mousse::fv::gradScheme<Type>::grad
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf,
  const word& name
) const
{
  typedef typename outerProduct<vector, Type>::type GradType;
  typedef GeometricField<GradType, fvPatchField, volMesh> GradFieldType;

  if (!this->mesh().changing() && this->mesh().cache(name)) {
    if (!mesh().objectRegistry::template foundObject<GradFieldType>(name)) {
      solution::cachePrintMessage("Calculating and caching", name, vsf);
      tmp<GradFieldType> tgGrad = calcGrad(vsf, name);
      regIOobject::store(tgGrad.ptr());
    }
    solution::cachePrintMessage("Retrieving", name, vsf);
    GradFieldType& gGrad = const_cast<GradFieldType&>
    (
      mesh().objectRegistry::template lookupObject<GradFieldType>(name)
    );
    if (gGrad.upToDate(vsf)) {
      return gGrad;
    } else {
      solution::cachePrintMessage("Deleting", name, vsf);
      gGrad.release();
      delete &gGrad;
      solution::cachePrintMessage("Recalculating", name, vsf);
      tmp<GradFieldType> tgGrad = calcGrad(vsf, name);
      solution::cachePrintMessage("Storing", name, vsf);
      regIOobject::store(tgGrad.ptr());
      GradFieldType& gGrad = const_cast<GradFieldType&>
      (
        mesh().objectRegistry::template lookupObject<GradFieldType>
        (
          name
        )
      );
      return gGrad;
    }
  } else {
    if (mesh().objectRegistry::template foundObject<GradFieldType>(name)) {
      GradFieldType& gGrad = const_cast<GradFieldType&>
      (
        mesh().objectRegistry::template lookupObject<GradFieldType>
        (
          name
        )
      );
      if (gGrad.ownedByRegistry()) {
        solution::cachePrintMessage("Deleting", name, vsf);
        gGrad.release();
        delete &gGrad;
      }
    }
    solution::cachePrintMessage("Calculating", name, vsf);
    return calcGrad(vsf, name);
  }
}


template<class Type>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<mousse::vector, Type>::type,
    mousse::fvPatchField,
    mousse::volMesh
  >
>
mousse::fv::gradScheme<Type>::grad
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf
) const
{
  return grad(vsf, "grad(" + vsf.name() + ')');
}


template<class Type>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<mousse::vector, Type>::type,
    mousse::fvPatchField,
    mousse::volMesh
  >
>
mousse::fv::gradScheme<Type>::grad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvsf
) const
{
  typedef typename outerProduct<vector, Type>::type GradType;
  typedef GeometricField<GradType, fvPatchField, volMesh> GradFieldType;
  tmp<GradFieldType> tgrad = grad(tvsf());
  tvsf.clear();
  return tgrad;
}

