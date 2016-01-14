// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "actuation_disk_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrix.hpp"
#include "geometric_one_field.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  DEFINE_TYPE_NAME_AND_DEBUG(actuationDiskSource, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    option,
    actuationDiskSource,
    dictionary
  );
}
}
// Private Member Functions 
void mousse::fv::actuationDiskSource::checkData() const
{
  if (magSqr(diskArea_) <= VSMALL)
  {
    FATAL_ERROR_IN("mousse::fv::actuationDiskSource::checkData()")
     << "diskArea is approximately zero"
     << exit(FatalIOError);
  }
  if (Cp_ <= VSMALL || Ct_ <= VSMALL)
  {
    FATAL_ERROR_IN("mousse::fv::actuationDiskSource::checkData()")
     << "Cp and Ct must be greater than zero"
     << exit(FatalIOError);
  }
  if (mag(diskDir_) < VSMALL)
  {
    FATAL_ERROR_IN("mousse::fv::actuationDiskSource::checkData()")
     << "disk direction vector is approximately zero"
     << exit(FatalIOError);
  }
  if (returnReduce(upstreamCellId_, maxOp<label>()) == -1)
  {
    FATAL_ERROR_IN("mousse::fv::actuationDiskSource::checkData()")
     << "upstream location " << upstreamPoint_  << " not found in mesh"
     << exit(FatalIOError);
  }
}
// Constructors 
mousse::fv::actuationDiskSource::actuationDiskSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  diskDir_(coeffs_.lookup("diskDir")),
  Cp_(readScalar(coeffs_.lookup("Cp"))),
  Ct_(readScalar(coeffs_.lookup("Ct"))),
  diskArea_(readScalar(coeffs_.lookup("diskArea"))),
  upstreamPoint_(coeffs_.lookup("upstreamPoint")),
  upstreamCellId_(-1)
{
  coeffs_.lookup("fieldNames") >> fieldNames_;
  applied_.setSize(fieldNames_.size(), false);
  Info<< "    - creating actuation disk zone: "
    << this->name() << endl;
  upstreamCellId_ = mesh.findCell(upstreamPoint_);
  checkData();
}
// Member Functions 
void mousse::fv::actuationDiskSource::addSup
(
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  const scalarField& cellsV = mesh_.V();
  vectorField& Usource = eqn.source();
  const vectorField& U = eqn.psi();
  if (V() > VSMALL)
  {
    addActuationDiskAxialInertialResistance
    (
      Usource,
      cells_,
      cellsV,
      geometricOneField(),
      U
    );
  }
}
void mousse::fv::actuationDiskSource::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  const scalarField& cellsV = mesh_.V();
  vectorField& Usource = eqn.source();
  const vectorField& U = eqn.psi();
  if (V() > VSMALL)
  {
    addActuationDiskAxialInertialResistance
    (
      Usource,
      cells_,
      cellsV,
      rho,
      U
    );
  }
}
bool mousse::fv::actuationDiskSource::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    coeffs_.readIfPresent("diskDir", diskDir_);
    coeffs_.readIfPresent("Cp", Cp_);
    coeffs_.readIfPresent("Ct", Ct_);
    coeffs_.readIfPresent("diskArea", diskArea_);
    checkData();
    return true;
  }
  else
  {
    return false;
  }
}
