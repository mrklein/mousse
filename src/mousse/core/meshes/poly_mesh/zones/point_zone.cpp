// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_zone.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_zone_mesh.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "demand_driven_data.hpp"
#include "sync_tools.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pointZone, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(pointZone, dictionary);
ADD_TO_RUN_TIME_SELECTION_TABLE(pointZone, pointZone, dictionary);

}

const char* const mousse::pointZone::labelsName = "pointLabels";


// Constructors 
mousse::pointZone::pointZone
(
  const word& name,
  const labelUList& addr,
  const label index,
  const pointZoneMesh& zm
)
:
  zone{name, addr, index},
  zoneMesh_{zm}
{}


mousse::pointZone::pointZone
(
  const word& name,
  const Xfer<labelList>& addr,
  const label index,
  const pointZoneMesh& zm
)
:
  zone{name, addr, index},
  zoneMesh_{zm}
{}


mousse::pointZone::pointZone
(
  const word& name,
  const dictionary& dict,
  const label index,
  const pointZoneMesh& zm
)
:
  zone{name, dict, this->labelsName, index},
  zoneMesh_{zm}
{}


mousse::pointZone::pointZone
(
  const pointZone& pz,
  const labelUList& addr,
  const label index,
  const pointZoneMesh& zm
)
:
  zone{pz, addr, index},
  zoneMesh_{zm}
{}


mousse::pointZone::pointZone
(
  const pointZone& pz,
  const Xfer<labelList>& addr,
  const label index,
  const pointZoneMesh& zm
)
:
  zone{pz, addr, index},
  zoneMesh_{zm}
{}


// Destructor 
mousse::pointZone::~pointZone()
{}


// Member Functions 
const mousse::pointZoneMesh& mousse::pointZone::zoneMesh() const
{
  return zoneMesh_;
}


mousse::label mousse::pointZone::whichPoint(const label globalPointID) const
{
  return zone::localID(globalPointID);
}


bool mousse::pointZone::checkDefinition(const bool report) const
{
  return zone::checkDefinition(zoneMesh_.mesh().points().size(), report);
}


bool mousse::pointZone::checkParallelSync(const bool report) const
{
  const polyMesh& mesh = zoneMesh().mesh();
  labelList maxZone{mesh.nPoints(), -1};
  labelList minZone{mesh.nPoints(), labelMax};
  FOR_ALL(*this, i) {
    label pointI = operator[](i);
    maxZone[pointI] = index();
    minZone[pointI] = index();
  }
  syncTools::syncPointList(mesh, maxZone, maxEqOp<label>(), label(-1));
  syncTools::syncPointList(mesh, minZone, minEqOp<label>(), labelMax);
  bool error = false;
  FOR_ALL(maxZone, pointI) {
    // Check point in same (or no) zone on all processors
    if ((maxZone[pointI] != -1
         || minZone[pointI] != labelMax)
        && (maxZone[pointI] != minZone[pointI])) {
      if (report && !error) {
        Info << " ***Problem with pointZone " << index()
          << " named " << name()
          << ". Point " << pointI
          << " at " << mesh.points()[pointI]
          << " is in zone "
          << (minZone[pointI] == labelMax ? -1 : minZone[pointI])
          << " on some processors and in zone "
          << maxZone[pointI]
          << " on some other processors." << nl
          << "(suppressing further warnings)"
          << endl;
      }
      error = true;
    }
  }
  return error;
}


void mousse::pointZone::writeDict(Ostream& os) const
{
  os << nl << name_ << nl << token::BEGIN_BLOCK << nl
    << "    type " << type() << token::END_STATEMENT << nl;
  writeEntry(this->labelsName, os);
  os << token::END_BLOCK << endl;
}


// Member Operators 
void mousse::pointZone::operator=(const pointZone& zn)
{
  clearAddressing();
  labelList::operator=(zn);
}


void mousse::pointZone::operator=(const labelUList& addr)
{
  clearAddressing();
  labelList::operator=(addr);
}


void mousse::pointZone::operator=(const Xfer<labelList>& addr)
{
  clearAddressing();
  labelList::operator=(addr);
}


// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const pointZone& zn)
{
  zn.write(os);
  os.check("Ostream& operator<<(Ostream&, const pointZone&");
  return os;
}

