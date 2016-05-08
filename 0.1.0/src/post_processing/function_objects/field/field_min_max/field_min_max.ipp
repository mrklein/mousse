// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_min_max.hpp"
#include "vol_fields.hpp"


template<class Type>
void mousse::fieldMinMax::output
(
  const word& fieldName,
  const word& outputName,
  const vector& minC,
  const vector& maxC,
  const label minProcI,
  const label maxProcI,
  const Type& minValue,
  const Type& maxValue
)
{
  OFstream& file = this->file();
  if (location_) {
    file << obr_.time().value();
    writeTabbed(file, fieldName);
    file << token::TAB << minValue << token::TAB << minC;
    if (Pstream::parRun()) {
      file << token::TAB << minProcI;
    }
    file << token::TAB << maxValue << token::TAB << maxC;
    if (Pstream::parRun()) {
      file << token::TAB << maxProcI;
    }
    file << endl;
    if (log_) {
      Info << "    min(" << outputName << ") = " << minValue
        << " at location " << minC;
    }
    if (Pstream::parRun()) {
      if (log_)
        Info << " on processor " << minProcI;
    }
    if (log_) {
      Info << nl << "    max(" << outputName << ") = " << maxValue
        << " at location " << maxC;
    }
    if (Pstream::parRun()) {
      if (log_) {
        Info << " on processor " << maxProcI;
      }
    }
  } else {
    file << token::TAB << minValue << token::TAB << maxValue;
    if (log_) {
      Info << "    min/max(" << outputName << ") = "
        << minValue << ' ' << maxValue;
    }
  }
  if (log_) {
    Info << endl;
  }
}


template<class Type>
void mousse::fieldMinMax::calcMinMaxFields
(
  const word& fieldName,
  const modeType& mode
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (!obr_.foundObject<fieldType>(fieldName))
    return;
  const label procI = Pstream::myProcNo();
  const fieldType& field = obr_.lookupObject<fieldType>(fieldName);
  const fvMesh& mesh = field.mesh();
  const volVectorField::GeometricBoundaryField& CfBoundary =
    mesh.C().boundaryField();
  switch (mode) {
    case mdMag:
      {
        const volScalarField magField(mag(field));
        const volScalarField::GeometricBoundaryField& magFieldBoundary =
          magField.boundaryField();
        scalarList minVs{Pstream::nProcs()};
        List<vector> minCs{Pstream::nProcs()};
        label minProcI = findMin(magField);
        minVs[procI] = magField[minProcI];
        minCs[procI] = field.mesh().C()[minProcI];
        labelList maxIs{Pstream::nProcs()};
        scalarList maxVs{Pstream::nProcs()};
        List<vector> maxCs{Pstream::nProcs()};
        label maxProcI = findMax(magField);
        maxVs[procI] = magField[maxProcI];
        maxCs[procI] = field.mesh().C()[maxProcI];
        FOR_ALL(magFieldBoundary, patchI) {
          const scalarField& mfp = magFieldBoundary[patchI];
          if (mfp.size()) {
            const vectorField& Cfp = CfBoundary[patchI];
            label minPI = findMin(mfp);
            if (mfp[minPI] < minVs[procI]) {
              minVs[procI] = mfp[minPI];
              minCs[procI] = Cfp[minPI];
            }
            label maxPI = findMax(mfp);
            if (mfp[maxPI] > maxVs[procI]) {
              maxVs[procI] = mfp[maxPI];
              maxCs[procI] = Cfp[maxPI];
            }
          }
        }
        Pstream::gatherList(minVs);
        Pstream::gatherList(minCs);
        Pstream::gatherList(maxVs);
        Pstream::gatherList(maxCs);
        if (Pstream::master()) {
          label minI = findMin(minVs);
          scalar minValue = minVs[minI];
          const vector& minC = minCs[minI];
          label maxI = findMax(maxVs);
          scalar maxValue = maxVs[maxI];
          const vector& maxC = maxCs[maxI];
          output
            (
              fieldName,
              word("mag(" + fieldName + ")"),
              minC,
              maxC,
              minI,
              maxI,
              minValue,
              maxValue
            );
        }
        break;
      }
    case mdCmpt:
      {
        const typename fieldType::GeometricBoundaryField&
          fieldBoundary = field.boundaryField();
        List<Type> minVs{Pstream::nProcs()};
        List<vector> minCs{Pstream::nProcs()};
        label minProcI = findMin(field);
        minVs[procI] = field[minProcI];
        minCs[procI] = field.mesh().C()[minProcI];
        Pstream::gatherList(minVs);
        Pstream::gatherList(minCs);
        List<Type> maxVs{Pstream::nProcs()};
        List<vector> maxCs{Pstream::nProcs()};
        label maxProcI = findMax(field);
        maxVs[procI] = field[maxProcI];
        maxCs[procI] = field.mesh().C()[maxProcI];
        FOR_ALL(fieldBoundary, patchI) {
          const Field<Type>& fp = fieldBoundary[patchI];
          if (fp.size()) {
            const vectorField& Cfp = CfBoundary[patchI];
            label minPI = findMin(fp);
            if (fp[minPI] < minVs[procI]) {
              minVs[procI] = fp[minPI];
              minCs[procI] = Cfp[minPI];
            }
            label maxPI = findMax(fp);
            if (fp[maxPI] > maxVs[procI]) {
              maxVs[procI] = fp[maxPI];
              maxCs[procI] = Cfp[maxPI];
            }
          }
        }
        Pstream::gatherList(minVs);
        Pstream::gatherList(minCs);
        Pstream::gatherList(maxVs);
        Pstream::gatherList(maxCs);
        if (Pstream::master()) {
          label minI = findMin(minVs);
          Type minValue = minVs[minI];
          const vector& minC = minCs[minI];
          label maxI = findMax(maxVs);
          Type maxValue = maxVs[maxI];
          const vector& maxC = maxCs[maxI];
          output
            (
              fieldName,
              fieldName,
              minC,
              maxC,
              minI,
              maxI,
              minValue,
              maxValue
            );
        }
        break;
      }
    default:
      {
        FATAL_ERROR_IN
        (
          "mousse::fieldMinMax::calcMinMaxFields"
          "("
          "const word&, "
          "const modeType&"
          ")"
        )
        << "Unknown min/max mode: " << modeTypeNames_[mode_]
        << exit(FatalError);
      }
  }
}

