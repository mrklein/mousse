// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::addSubtract::writeAddSubtractValue
(
  const IOobject& baseHeader,
  const string& valueStr,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (baseHeader.headerClassName() == fieldType::typeName)
  {
    if (resultName_ == "")
    {
      if (calcMode_ == ADD)
      {
        resultName_ = baseHeader.name() + "_add_value";
      }
      else
      {
        resultName_ = baseHeader.name() + "_subtract_value";
      }
    }
    Type value;
    IStringStream(valueStr)() >> value;
    Info<< "    Reading " << baseHeader.name() << endl;
    fieldType baseField(baseHeader, mesh);
    fieldType newField
    (
      IOobject
      (
        resultName_,
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ
      ),
      baseField
    );
    Info<< "    Calculating " << resultName_ << endl;
    if (calcMode_ == ADD)
    {
      newField == baseField
        + dimensioned<Type>("value", baseField.dimensions(), value);
    }
    else
    {
      newField == baseField
        - dimensioned<Type>("value", baseField.dimensions(), value);
    }
    newField.write();
    processed = true;
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
