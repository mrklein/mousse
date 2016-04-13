// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "find_ref_cell.hpp"
#include "pstream_reduce_ops.hpp"


// Global Functions 
void mousse::setRefCell
(
  const volScalarField& field,
  const volScalarField& fieldRef,
  const dictionary& dict,
  label& refCelli,
  scalar& refValue,
  const bool forceReference
)
{
  if (!fieldRef.needReference() && !forceReference)
    return;
  word refCellName = field.name() + "RefCell";
  word refPointName = field.name() + "RefPoint";
  word refValueName = field.name() + "RefValue";
  if (dict.found(refCellName)) {
    if (Pstream::master()) {
      refCelli = readLabel(dict.lookup(refCellName));
      if (refCelli < 0 || refCelli >= field.mesh().nCells()) {
        FATAL_IO_ERROR_IN
        (
          "void mousse::setRefCell\n"
          "    (\n"
          "        const volScalarField&,\n"
          "        const volScalarField&,\n"
          "        const dictionary&,\n"
          "        label& scalar&,\n"
          "        bool\n"
          ")",
          dict
        )
        << "Illegal master cellID " << refCelli
        << ". Should be 0.." << field.mesh().nCells()
        << exit(FatalIOError);
      }
    } else {
      refCelli = -1;
    }
  } else if (dict.found(refPointName)) {
    point refPointi{dict.lookup(refPointName)};
    // Try fast approximate search avoiding octree construction
    refCelli = field.mesh().findCell(refPointi, polyMesh::FACE_PLANES);
    label hasRef = (refCelli >= 0 ? 1 : 0);
    label sumHasRef = returnReduce<label>(hasRef, sumOp<label>());
    // If reference cell no found use octree search
    // with cell tet-decompositoin
    if (sumHasRef != 1) {
      refCelli = field.mesh().findCell(refPointi);
      hasRef = (refCelli >= 0 ? 1 : 0);
      sumHasRef = returnReduce<label>(hasRef, sumOp<label>());
    }
    if (sumHasRef != 1) {
      FATAL_IO_ERROR_IN
      (
        "void mousse::setRefCell\n"
        "    (\n"
        "        const volScalarField&,\n"
        "        const volScalarField&,\n"
        "        const dictionary&,\n"
        "        label& scalar&,\n"
        "        bool\n"
        "    )",
        dict
      )
      << "Unable to set reference cell for field " << field.name()
      << nl << "    Reference point " << refPointName
      << " " << refPointi
      << " found on " << sumHasRef << " domains (should be one)"
      << nl << exit(FatalIOError);
    }
  } else {
    FATAL_IO_ERROR_IN
    (
      "void mousse::setRefCell\n"
      "    (\n"
      "        const volScalarField&,\n"
      "        const volScalarField&,\n"
      "        const dictionary&,\n"
      "        label& scalar&,\n"
      "        bool\n"
      "    )",
      dict
    )
    << "Unable to set reference cell for field " << field.name()
    << nl
    << "    Please supply either " << refCellName
    << " or " << refPointName << nl << exit(FatalIOError);
  }
  refValue = readScalar(dict.lookup(refValueName));
}


void mousse::setRefCell
(
  const volScalarField& field,
  const dictionary& dict,
  label& refCelli,
  scalar& refValue,
  const bool forceReference
)
{
  setRefCell(field, field, dict, refCelli, refValue, forceReference);
}


mousse::scalar mousse::getRefCellValue
(
  const volScalarField& field,
  const label refCelli
)
{
  scalar refCellValue = (refCelli >= 0 ? field[refCelli] : 0.0);
  return returnReduce(refCellValue, sumOp<scalar>());
}

