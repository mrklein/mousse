// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ignition_site.hpp"
#include "engine_time.hpp"


// Constructors 
mousse::ignitionSite::ignitionSite
(
  Istream& is,
  const Time& db,
  const fvMesh& mesh
)
:
  db_{db},
  mesh_{mesh},
  ignitionSiteDict_{is},
  location_{ignitionSiteDict_.lookup("location")},
  diameter_{readScalar(ignitionSiteDict_.lookup("diameter"))},
  time_
  {
    db_.userTimeToTime
    (
      readScalar(ignitionSiteDict_.lookup("start"))
    )
  },
  duration_
  {
    db_.userTimeToTime
    (
      readScalar(ignitionSiteDict_.lookup("duration"))
    )
  },
  strength_{readScalar(ignitionSiteDict_.lookup("strength"))},
  timeIndex_{db_.timeIndex()}
{
  // Check state of Istream
  is.check("ignitionSite::ignitionSite(Istream&)");
  findIgnitionCells(mesh_);
}


mousse::ignitionSite::ignitionSite
(
  Istream& is,
  const engineTime& edb,
  const fvMesh& mesh
)
:
  db_{edb},
  mesh_{mesh},
  ignitionSiteDict_{is},
  location_{ignitionSiteDict_.lookup("location")},
  diameter_{readScalar(ignitionSiteDict_.lookup("diameter"))},
  time_
  {
    db_.userTimeToTime
    (
      edb.degToTime(readScalar(ignitionSiteDict_.lookup("start")))
    )
  },
  duration_
  {
    db_.userTimeToTime
    (
      edb.degToTime(readScalar(ignitionSiteDict_.lookup("duration")))
    )
  },
  strength_{readScalar(ignitionSiteDict_.lookup("strength"))},
  timeIndex_{db_.timeIndex()}
{
  // Check state of Istream
  is.check("ignitionSite::ignitionSite(Istream&)");
  findIgnitionCells(mesh_);
}

