// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_zone.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "cell_zone_mesh.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "iostream.hpp"
#include "demand_driven_data.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cellZone, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(cellZone, dictionary);
ADD_TO_RUN_TIME_SELECTION_TABLE(cellZone, cellZone, dictionary);

}

const char * const mousse::cellZone::labelsName = "cellLabels";


// Constructors
mousse::cellZone::cellZone
(
  const word& name,
  const labelUList& addr,
  const label index,
  const cellZoneMesh& zm
)
:
  zone{name, addr, index},
  zoneMesh_{zm}
{}


mousse::cellZone::cellZone
(
  const word& name,
  const Xfer<labelList>& addr,
  const label index,
  const cellZoneMesh& zm
)
:
  zone{name, addr, index},
  zoneMesh_{zm}
{}


mousse::cellZone::cellZone
(
  const word& name,
  const dictionary& dict,
  const label index,
  const cellZoneMesh& zm
)
:
  zone{name, dict, this->labelsName, index},
  zoneMesh_{zm}
{}


mousse::cellZone::cellZone
(
  const cellZone& cz,
  const labelUList& addr,
  const label index,
  const cellZoneMesh& zm
)
:
  zone{cz, addr, index},
  zoneMesh_{zm}
{}


mousse::cellZone::cellZone
(
  const cellZone& cz,
  const Xfer<labelList>& addr,
  const label index,
  const cellZoneMesh& zm
)
:
  zone{cz, addr, index},
  zoneMesh_{zm}
{}


// Destructor
mousse::cellZone::~cellZone()
{}


// Member Functions
mousse::label mousse::cellZone::whichCell(const label globalCellID) const
{
  return zone::localID(globalCellID);
}


const mousse::cellZoneMesh& mousse::cellZone::zoneMesh() const
{
  return zoneMesh_;
}


bool mousse::cellZone::checkDefinition(const bool report) const
{
  return zone::checkDefinition(zoneMesh_.mesh().nCells(), report);
}


void mousse::cellZone::writeDict(Ostream& os) const
{
  os << nl << name() << nl << token::BEGIN_BLOCK << nl
    << "    type " << type() << token::END_STATEMENT << nl;
  writeEntry(this->labelsName, os);
  os  << token::END_BLOCK << endl;
}


// Member Operators
void mousse::cellZone::operator=(const cellZone& zn)
{
  clearAddressing();
  labelList::operator=(zn);
}


void mousse::cellZone::operator=(const labelUList& addr)
{
  clearAddressing();
  labelList::operator=(addr);
}


void mousse::cellZone::operator=(const Xfer<labelList>& addr)
{
  clearAddressing();
  labelList::operator=(addr);
}


// Ostream Operator
mousse::Ostream& mousse::operator<<(Ostream& os, const cellZone& zn)
{
  zn.write(os);
  os.check("Ostream& operator<<(Ostream&, const cellZone&");
  return os;
}
