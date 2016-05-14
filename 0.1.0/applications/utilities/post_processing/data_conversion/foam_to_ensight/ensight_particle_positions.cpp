// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_particle_positions.hpp"
#include "fv_mesh.hpp"
#include "passive_particle.hpp"
#include "cloud.hpp"
#include "ofstream.hpp"
#include "iomanip.hpp"
#include "itoa.hpp"


using namespace mousse;


// Global Functions 
void ensightParticlePositions
(
  const fvMesh& mesh,
  const fileName& postProcPath,
  const word& timeFile,
  const word& cloudName,
  const bool dataExists
)
{
  if (dataExists) {
    Info << "Converting cloud " << cloudName << " positions" <<  endl;
  } else {
    Info << "Creating empty cloud " << cloudName << " positions" << endl;
  }
  const Time& runTime = mesh.time();
  fileName ensightFileName{timeFile + "." + cloudName};
  OFstream ensightFile
  {
    postProcPath/ensightFileName,
    runTime.writeFormat(),
    runTime.writeVersion(),
    runTime.writeCompression()
  };
  // Output header
  ensightFile
    << cloudName.c_str() << nl
    << "particle coordinates" << nl;
  if (dataExists) {
    Cloud<passiveParticle> parcels{mesh, cloudName, false};
    // Set Format
    ensightFile.setf(ios_base::scientific, ios_base::floatfield);
    ensightFile.precision(5);
    ensightFile << setw(8) << parcels.size() << nl;
    label nParcels = 0;
    // Output positions
    FOR_ALL_CONST_ITER(Cloud<passiveParticle>, parcels, elmnt) {
      const vector& p = elmnt().position();
      ensightFile
        << setw(8) << ++nParcels
        << setw(12) << p.x() << setw(12) << p.y() << setw(12) << p.z()
        << nl;
    }
  } else {
    label nParcels = 0;
    ensightFile<< setw(8) << nParcels << nl;
  }
}

