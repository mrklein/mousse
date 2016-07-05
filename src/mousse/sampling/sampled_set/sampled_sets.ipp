// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets.hpp"
#include "vol_fields.hpp"
#include "list_list_ops.hpp"


// Private Member Functions 
template<class Type>
mousse::sampledSets::volFieldSampler<Type>::volFieldSampler
(
  const word& interpolationScheme,
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const PtrList<sampledSet>& samplers
)
:
  List<Field<Type>>{samplers.size()},
  name_{field.name()}
{
  autoPtr<interpolation<Type>> interpolator
  {
    interpolation<Type>::New(interpolationScheme, field)
  };
  FOR_ALL(samplers, setI) {
    Field<Type>& values = this->operator[](setI);
    const sampledSet& samples = samplers[setI];
    values.setSize(samples.size());
    FOR_ALL(samples, sampleI) {
      const point& samplePt = samples[sampleI];
      label cellI = samples.cells()[sampleI];
      label faceI = samples.faces()[sampleI];
      if (cellI == -1 && faceI == -1) {
        // Special condition for illegal sampling points
        values[sampleI] = pTraits<Type>::max;
      } else {
        values[sampleI] = interpolator().interpolate(samplePt, cellI, faceI);
      }
    }
  }
}


template<class Type>
mousse::sampledSets::volFieldSampler<Type>::volFieldSampler
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const PtrList<sampledSet>& samplers
)
:
  List<Field<Type>>{samplers.size()},
  name_{field.name()}
{
  FOR_ALL(samplers, setI) {
    Field<Type>& values = this->operator[](setI);
    const sampledSet& samples = samplers[setI];
    values.setSize(samples.size());
    FOR_ALL(samples, sampleI) {
      label cellI = samples.cells()[sampleI];
      if (cellI ==-1) {
        values[sampleI] = pTraits<Type>::max;
      } else {
        values[sampleI] = field[cellI];
      }
    }
  }
}


template<class Type>
mousse::sampledSets::volFieldSampler<Type>::volFieldSampler
(
  const List<Field<Type>>& values,
  const word& name
)
:
  List<Field<Type>>{values},
  name_{name}
{}


template<class Type>
void mousse::sampledSets::writeSampleFile
(
  const coordSet& masterSampleSet,
  const PtrList<volFieldSampler<Type>>& masterFields,
  const label setI,
  const fileName& timeDir,
  const writer<Type>& formatter
)
{
  wordList valueSetNames{masterFields.size()};
  List<const Field<Type>*> valueSets{masterFields.size()};
  FOR_ALL(masterFields, fieldi) {
    valueSetNames[fieldi] = masterFields[fieldi].name();
    valueSets[fieldi] = &masterFields[fieldi][setI];
  }
  fileName fName{timeDir/formatter.getFileName(masterSampleSet, valueSetNames)};
  OFstream ofs{fName};
  if (ofs.opened()) {
    formatter.write(masterSampleSet, valueSetNames, valueSets, ofs);
  } else {
    WARNING_IN
    (
      "void mousse::sampledSets::writeSampleFile"
      "("
        "const coordSet&, "
        "const PtrList<volFieldSampler<Type>>&, "
        "const label, "
        "const fileName&, "
        "const writer<Type>&"
      ")"
    )
    << "File " << ofs.name() << " could not be opened. "
    << "No data will be written" << endl;
  }
}


template<class T>
void mousse::sampledSets::combineSampledValues
(
  const PtrList<volFieldSampler<T>>& sampledFields,
  const labelListList& indexSets,
  PtrList<volFieldSampler<T>>& masterFields
)
{
  FOR_ALL(sampledFields, fieldi) {
    List<Field<T>> masterValues{indexSets.size()};
    FOR_ALL(indexSets, setI) {
      // Collect data from all processors
      List<Field<T>> gatheredData{Pstream::nProcs()};
      gatheredData[Pstream::myProcNo()] = sampledFields[fieldi][setI];
      Pstream::gatherList(gatheredData);
      if (Pstream::master()) {
        Field<T> allData
        {
          ListListOps::combine<Field<T>>(gatheredData,
                                         mousse::accessOp<Field<T>>())
        };
        masterValues[setI] = UIndirectList<T>{allData, indexSets[setI]}();
      }
    }
    masterFields.set
    (
      fieldi,
      new volFieldSampler<T>
      {
        masterValues,
        sampledFields[fieldi].name()
      }
    );
  }
}


template<class Type>
void mousse::sampledSets::sampleAndWrite
(
  fieldGroup<Type>& fields
)
{
  if (!fields.size()) 
    return;
  bool interpolate = interpolationScheme_ != "cell";
  // Create or use existing writer
  if (fields.formatter.empty()) {
    fields = writeFormat_;
  }
  // Storage for interpolated values
  PtrList<volFieldSampler<Type>> sampledFields{fields.size()};
  FOR_ALL(fields, fieldi) {
    if (Pstream::master() && verbose_) {
      Pout << "sampledSets::sampleAndWrite: " << fields[fieldi] << endl;
    }
    if (loadFromFiles_) {
      GeometricField<Type, fvPatchField, volMesh> vf
      {
        {
          fields[fieldi],
          mesh_.time().timeName(),
          mesh_,
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_
      };
      if (interpolate) {
        sampledFields.set
        (
          fieldi,
          new volFieldSampler<Type>{interpolationScheme_, vf, *this}
        );
      } else {
        sampledFields.set
        (
          fieldi,
          new volFieldSampler<Type>{vf, *this}
        );
      }
    } else {
      if (interpolate) {
        sampledFields.set
        (
          fieldi,
          new volFieldSampler<Type>
          {
            interpolationScheme_,
            mesh_.lookupObject<GeometricField<Type, fvPatchField, volMesh>>(fields[fieldi]),
            *this
          }
        );
      } else {
        sampledFields.set
        (
          fieldi,
          new volFieldSampler<Type>
          {
            mesh_.lookupObject<GeometricField<Type, fvPatchField, volMesh>>(fields[fieldi]),
            *this
          }
        );
      }
    }
  }
  // Combine sampled fields from processors.
  // Note: only master results are valid
  PtrList<volFieldSampler<Type>> masterFields{sampledFields.size()};
  combineSampledValues(sampledFields, indexSets_, masterFields);
  if (Pstream::master()) {
    FOR_ALL(masterSampledSets_, setI) {
      writeSampleFile
      (
        masterSampledSets_[setI],
        masterFields,
        setI,
        outputPath_/mesh_.time().timeName(),
        fields.formatter()
      );
    }
  }
}

