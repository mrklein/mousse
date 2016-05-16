// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_source.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

template<>
const char* NamedEnum<fieldValues::cellSource::sourceType, 2>::names[] =
{
  "cellZone",
  "all"
};

template<>
const char* NamedEnum<fieldValues::cellSource::operationType, 11>::names[] =
{
  "none",
  "sum",
  "sumMag",
  "average",
  "weightedAverage",
  "volAverage",
  "weightedVolAverage",
  "volIntegrate",
  "min",
  "max",
  "CoV"
};

namespace fieldValues
{
  DEFINE_TYPE_NAME_AND_DEBUG(cellSource, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(fieldValue, cellSource, dictionary);
}

}

const mousse::NamedEnum<mousse::fieldValues::cellSource::sourceType, 2>
  mousse::fieldValues::cellSource::sourceTypeNames_;

const mousse::NamedEnum<mousse::fieldValues::cellSource::operationType, 11>
  mousse::fieldValues::cellSource::operationTypeNames_;


// Private Member Functions 
void mousse::fieldValues::cellSource::setCellZoneCells()
{
  switch (source_) {
    case stCellZone:
      {
        dict().lookup("sourceName") >> sourceName_;
        label zoneId = mesh().cellZones().findZoneID(sourceName_);
        if (zoneId < 0) {
          FATAL_ERROR_IN("cellSource::cellSource::setCellZoneCells()")
            << "Unknown cell zone name: " << sourceName_
            << ". Valid cell zones are: " << mesh().cellZones().names()
            << nl << exit(FatalError);
        }
        cellId_ = mesh().cellZones()[zoneId];
        nCells_ = returnReduce(cellId_.size(), sumOp<label>());
        break;
      }
    case stAll:
      {
        cellId_ = identity(mesh().nCells());
        nCells_ = returnReduce(cellId_.size(), sumOp<label>());
        break;
      }
    default:
      {
        FATAL_ERROR_IN("cellSource::setCellZoneCells()")
          << "Unknown source type. Valid source types are:"
          << sourceTypeNames_ << nl << exit(FatalError);
      }
  }
  if (debug) {
    Pout << "Selected source size = " << cellId_.size() << endl;
  }
}


mousse::scalar mousse::fieldValues::cellSource::volume() const
{
  return gSum(filterField(mesh().V()));
}


// Protected Member Functions 
void mousse::fieldValues::cellSource::initialise(const dictionary& dict)
{
  setCellZoneCells();
  if (nCells_ == 0) {
    WARNING_IN
    (
      "mousse::fieldValues::cellSource::initialise(const dictionary&)"
    )
    << type() << " " << name_ << ": "
    << sourceTypeNames_[source_] << "(" << sourceName_ << "):" << nl
    << "    Source has no cells - deactivating" << endl;
    active_ = false;
    return;
  }
  volume_ = volume();
  Info << type() << " " << name_ << ":"
    << sourceTypeNames_[source_] << "(" << sourceName_ << "):" << nl
    << "    total cells  = " << nCells_ << nl
    << "    total volume = " << volume_
    << nl << endl;
  if (dict.readIfPresent("weightField", weightFieldName_)) {
    Info << "    weight field = " << weightFieldName_;
  }
  Info << nl << endl;
}


void mousse::fieldValues::cellSource::writeFileHeader(const label /*i*/)
{
  writeCommented(file(), "Source : ");
  file() << sourceTypeNames_[source_] << " " << sourceName_ << endl;
  writeCommented(file(), "Cells  : ");
  file() << nCells_ << endl;
  writeCommented(file(), "Volume : ");
  file() << volume_ << endl;
  writeCommented(file(), "Time");
  if (writeVolume_) {
    file() << tab << "Volume";
  }
  FOR_ALL(fields_, i) {
    file()
      << tab << operationTypeNames_[operation_]
      << "(" << fields_[i] << ")";
  }
  file() << endl;
}


// Constructors 
mousse::fieldValues::cellSource::cellSource
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  fieldValue{name, obr, dict, typeName, loadFromFiles},
  source_{sourceTypeNames_.read(dict.lookup("source"))},
  operation_{operationTypeNames_.read(dict.lookup("operation"))},
  nCells_{0},
  cellId_(),
  weightFieldName_{"none"},
  writeVolume_{dict.lookupOrDefault("writeVolume", false)}
{
  read(dict);
}


// Destructor 
mousse::fieldValues::cellSource::~cellSource()
{}


// Member Functions 
void mousse::fieldValues::cellSource::read(const dictionary& dict)
{
  fieldValue::read(dict);
  if (active_) {
    // no additional info to read
    initialise(dict);
  }
}


void mousse::fieldValues::cellSource::write()
{
  fieldValue::write();
  if (!active_)
    return;
  if (Pstream::master()) {
    file() << obr_.time().value();
  }
  if (writeVolume_) {
    volume_ = volume();
    if (Pstream::master()) {
      file() << tab << volume_;
    }
    if (log_) {
      Info << "    total volume = " << volume_ << endl;
    }
  }
  FOR_ALL(fields_, i) {
    const word& fieldName = fields_[i];
    bool processed = false;
    processed = processed || writeValues<scalar>(fieldName);
    processed = processed || writeValues<vector>(fieldName);
    processed = processed || writeValues<sphericalTensor>(fieldName);
    processed = processed || writeValues<symmTensor>(fieldName);
    processed = processed || writeValues<tensor>(fieldName);
    if (!processed) {
      WARNING_IN("void mousse::fieldValues::cellSource::write()")
        << "Requested field " << fieldName
        << " not found in database and not processed"
        << endl;
    }
  }
  if (Pstream::master()) {
    file()<< endl;
  }
  if (log_) {
    Info << endl;
  }
}

