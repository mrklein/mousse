// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::addSubtract::writeAddSubtractField
(
  const IOobject& baseHeader,
  const IOobject& addHeader,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if
  (
    baseHeader.headerClassName() == fieldType::typeName
  && baseHeader.headerClassName() == addHeader.headerClassName()
  )
  {
    if (resultName_ == "")
    {
      if (calcMode_ == ADD)
      {
        resultName_ = baseHeader.name() + "_add_" + addHeader.name();
      }
      else
      {
        resultName_ = baseHeader.name() + "_subtract_"
          + addHeader.name();
      }
    }
    Info<< "    Reading " << baseHeader.name() << endl;
    fieldType baseField(baseHeader, mesh);
    Info<< "    Reading " << addHeader.name() << endl;
    fieldType addField(addHeader, mesh);
    if (baseField.dimensions() == addField.dimensions())
    {
      Info<< "    Calculating " << resultName_ << endl;
      fieldType newField
      (
        IOobject
        (
          resultName_,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ
        ),
        calcMode_ == ADD
       ? (baseField + addField)()
       : (baseField - addField)()
      );
      newField.write();
    }
    else
    {
      Info<< "    Cannot calculate " << resultName_ << nl
        << "    - inconsistent dimensions: "
        << baseField.dimensions() << " - " << addField.dimensions()
        << endl;
    }
    processed = true;
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
