// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "topo_set_source.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "vol_fields.hpp"


using namespace mousse;


template<class Type>
bool setCellFieldType
(
  const word& fieldTypeDesc,
  const fvMesh& mesh,
  const labelList& selectedCells,
  Istream& fieldValueStream
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (fieldTypeDesc != fieldType::typeName + "Value") {
    return false;
  }
  word fieldName{fieldValueStream};
  // Check the current time directory
  IOobject fieldHeader
  {
    fieldName,
    mesh.time().timeName(),
    mesh,
    IOobject::MUST_READ
  };
  // Check the "constant" directory
  if (!fieldHeader.headerOk()) {
    fieldHeader =
      IOobject
      {
        fieldName,
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ
      };
  }
  // Check field exists
  if (fieldHeader.headerOk()) {
    Info << "    Setting internal values of "
      << fieldHeader.headerClassName()
      << " " << fieldName << endl;
    fieldType field{fieldHeader, mesh};
    const Type& value = pTraits<Type>(fieldValueStream);
    if (selectedCells.size() == field.size()) {
      field.internalField() = value;
    } else {
      FOR_ALL(selectedCells, celli) {
        field[selectedCells[celli]] = value;
      }
    }
    FOR_ALL(field.boundaryField(), patchi) {
      field.boundaryField()[patchi] =
        field.boundaryField()[patchi].patchInternalField();
    }
    if (!field.write()) {
      FATAL_ERROR_IN
      (
        "void setCellFieldType"
        "(const fvMesh& mesh, const labelList& selectedCells,"
        "Istream& fieldValueStream)"
      )
      << "Failed writing field " << fieldName << endl;
    }
  } else {
    WARNING_IN
    (
      "void setCellFieldType"
      "(const fvMesh& mesh, const labelList& selectedCells,"
      "Istream& fieldValueStream)"
    )
    << "Field " << fieldName << " not found" << endl;
    // Consume value
    (void)pTraits<Type>(fieldValueStream);
  }
  return true;
}


class setCellField
{
public:
  setCellField()
  {}
  autoPtr<setCellField> clone() const
  {
    return autoPtr<setCellField>{new setCellField{}};
  }
  class iNew
  {
    const fvMesh& mesh_;
    const labelList& selectedCells_;
  public:
    iNew(const fvMesh& mesh, const labelList& selectedCells)
    :
      mesh_{mesh},
      selectedCells_{selectedCells}
    {}
    autoPtr<setCellField> operator()(Istream& vals) const
    {
      word fT{vals};
      const auto& sC = selectedCells_;
      if (!(setCellFieldType<scalar>(fT, mesh_, sC, vals)
            || setCellFieldType<vector>(fT, mesh_, sC, vals)
            || setCellFieldType<sphericalTensor>(fT, mesh_, sC, vals)
            || setCellFieldType<symmTensor>(fT, mesh_, sC, vals)
            || setCellFieldType<tensor>(fT, mesh_, sC, vals))) {
        WARNING_IN("setCellField::iNew::operator()(Istream& is)")
          << "field type " << fT << " not currently supported"
          << endl;
      }
      return autoPtr<setCellField>{new setCellField{}};
    }
  };
};


template<class Type>
bool setFaceFieldType
(
  const word& fieldTypeDesc,
  const fvMesh& mesh,
  const labelList& selectedFaces,
  Istream& fieldValueStream
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (fieldTypeDesc != fieldType::typeName + "Value") {
    return false;
  }
  word fieldName{fieldValueStream};
  // Check the current time directory
  IOobject fieldHeader
  {
    fieldName,
    mesh.time().timeName(),
    mesh,
    IOobject::MUST_READ
  };
  // Check the "constant" directory
  if (!fieldHeader.headerOk()) {
    fieldHeader =
      IOobject
      {
        fieldName,
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ
      };
  }
  // Check field exists
  if (fieldHeader.headerOk()) {
    Info << "    Setting patchField values of "
      << fieldHeader.headerClassName()
      << " " << fieldName << endl;
    fieldType field{fieldHeader, mesh};
    const Type& value = pTraits<Type>(fieldValueStream);
    // Create flat list of selected faces and their value.
    Field<Type> allBoundaryValues{mesh.nFaces() - mesh.nInternalFaces()};
    FOR_ALL(field.boundaryField(), patchi) {
      SubField<Type>
      {
        allBoundaryValues,
        field.boundaryField()[patchi].size(),
        field.boundaryField()[patchi].patch().start() - mesh.nInternalFaces()
      }.assign(field.boundaryField()[patchi]);
    }
    // Override
    bool hasWarned = false;
    labelList nChanged
    {
      returnReduce(field.boundaryField().size(), maxOp<label>()),
      0
    };
    FOR_ALL(selectedFaces, i) {
      label facei = selectedFaces[i];
      if (mesh.isInternalFace(facei)) {
        if (!hasWarned) {
          hasWarned = true;
          WARNING_IN("setFaceFieldType(..)")
            << "Ignoring internal face " << facei
            << ". Suppressing further warnings." << endl;
        }
      } else {
        label bFaceI = facei-mesh.nInternalFaces();
        allBoundaryValues[bFaceI] = value;
        nChanged[mesh.boundaryMesh().patchID()[bFaceI]]++;
      }
    }
    Pstream::listCombineGather(nChanged, plusEqOp<label>());
    Pstream::listCombineScatter(nChanged);
    // Reassign.
    FOR_ALL(field.boundaryField(), patchi) {
      if (nChanged[patchi] > 0) {
        Info << "    On patch "
          << field.boundaryField()[patchi].patch().name()
          << " set " << nChanged[patchi] << " values" << endl;
        field.boundaryField()[patchi] == SubField<Type>
        (
          allBoundaryValues,
          field.boundaryField()[patchi].size(),
          field.boundaryField()[patchi].patch().start()
          - mesh.nInternalFaces()
        );
      }
    }
    if (!field.write()) {
      FATAL_ERROR_IN
      (
        "void setFaceFieldType"
        "(const fvMesh& mesh, const labelList& selectedFaces,"
        "Istream& fieldValueStream)"
      )
      << "Failed writing field " << field.name() << exit(FatalError);
    }
  } else {
    WARNING_IN
    (
      "void setFaceFieldType"
      "(const fvMesh& mesh, const labelList& selectedFaces,"
      "Istream& fieldValueStream)"
    )
    << "Field " << fieldName << " not found" << endl;
    // Consume value
    (void)pTraits<Type>(fieldValueStream);
  }
  return true;
}


class setFaceField
{
public:
  setFaceField()
  {}
  autoPtr<setFaceField> clone() const
  {
    return autoPtr<setFaceField>{new setFaceField{}};
  }
  class iNew
  {
    const fvMesh& mesh_;
    const labelList& selectedFaces_;
  public:
    iNew(const fvMesh& mesh, const labelList& selectedFaces)
    :
      mesh_{mesh},
      selectedFaces_{selectedFaces}
    {}
    autoPtr<setFaceField> operator()(Istream& vals) const
    {
      word fT{vals};
      if (!(setFaceFieldType<scalar>(fT, mesh_, selectedFaces_, vals)
            || setFaceFieldType<vector>(fT, mesh_, selectedFaces_, vals)
            || setFaceFieldType<sphericalTensor>(fT, mesh_, selectedFaces_, vals)
            || setFaceFieldType<symmTensor>(fT, mesh_, selectedFaces_, vals)
            || setFaceFieldType<tensor>(fT, mesh_, selectedFaces_, vals))) {
        WARNING_IN("setFaceField::iNew::operator()(Istream& is)")
          << "field type " << fT << " not currently supported"
          << endl;
      }
      return autoPtr<setFaceField>{new setFaceField{}};
    }
  };
};


int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_mesh.inc"
  Info << "Reading setFieldsDict\n" << endl;
  IOdictionary setFieldsDict
  {
    {
      "setFieldsDict",
      runTime.system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  if (setFieldsDict.found("defaultFieldValues")) {
    Info << "Setting field default values" << endl;
    PtrList<setCellField> defaultFieldValues
    {
      setFieldsDict.lookup("defaultFieldValues"),
      setCellField::iNew(mesh, labelList{mesh.nCells()})
    };
    Info << endl;
  }
  Info << "Setting field region values" << endl;
  PtrList<entry> regions{setFieldsDict.lookup("regions")};
  FOR_ALL(regions, regionI) {
    const entry& region = regions[regionI];
    autoPtr<topoSetSource> source =
      topoSetSource::New(region.keyword(), mesh, region.dict());
    if (source().setType() == topoSetSource::CELLSETSOURCE) {
      cellSet selectedCellSet
      {
        mesh,
        "cellSet",
        mesh.nCells()/10+1  // Reasonable size estimate.
      };
      source->applyToSet
      (
        topoSetSource::NEW,
        selectedCellSet
      );
      PtrList<setCellField> fieldValues
      {
        region.dict().lookup("fieldValues"),
        setCellField::iNew(mesh, selectedCellSet.toc())
      };
    } else if (source().setType() == topoSetSource::FACESETSOURCE) {
      faceSet selectedFaceSet
      {
        mesh,
        "faceSet",
        (mesh.nFaces()-mesh.nInternalFaces())/10+1
      };
      source->applyToSet
      (
        topoSetSource::NEW,
        selectedFaceSet
      );
      PtrList<setFaceField> fieldValues
      {
        region.dict().lookup("fieldValues"),
        setFaceField::iNew(mesh, selectedFaceSet.toc())
      };
    }
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

