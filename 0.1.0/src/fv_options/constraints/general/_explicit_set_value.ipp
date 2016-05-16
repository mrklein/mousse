// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_explicit_set_value.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "dimensioned_field.hpp"


// Protected Member Functions 
template<class Type>
void mousse::fv::ExplicitSetValue<Type>::setFieldData(const dictionary& dict)
{
  fieldNames_.setSize(dict.toc().size());
  injectionRate_.setSize(fieldNames_.size());
  applied_.setSize(fieldNames_.size(), false);
  label i = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter) {
    fieldNames_[i] = iter().keyword();
    dict.lookup(iter().keyword()) >> injectionRate_[i];
    i++;
  }
}


// Constructors 
template<class Type>
mousse::fv::ExplicitSetValue<Type>::ExplicitSetValue
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption{name, modelType, dict, mesh},
  injectionRate_{}
{
  read(dict);
}


// Member Functions 
template<class Type>
void mousse::fv::ExplicitSetValue<Type>::constrain
(
  fvMatrix<Type>& eqn,
  const label fieldI
)
{
  if (debug) {
    Info << "ExplicitSetValue<"<< pTraits<Type>::typeName
      << ">::constrain for source " << name_ << endl;
  }
  List<Type> values{cells_.size(), injectionRate_[fieldI]};
  eqn.setValues(cells_, values);
}

