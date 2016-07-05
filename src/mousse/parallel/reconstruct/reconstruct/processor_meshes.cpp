// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_meshes.hpp"
#include "time.hpp"
#include "primitive_mesh.hpp"


// Private Member Functions 
void mousse::processorMeshes::read()
{
  // Make sure to clear (and hence unregister) any previously loaded meshes
  // and fields
  FOR_ALL(databases_, procI) {
    meshes_.set(procI, NULL);
    pointProcAddressing_.set(procI, NULL);
    faceProcAddressing_.set(procI, NULL);
    cellProcAddressing_.set(procI, NULL);
    boundaryProcAddressing_.set(procI, NULL);
  }
  FOR_ALL(databases_, procI) {
    meshes_.set
    (
      procI,
      new fvMesh
      {
        IOobject
        {
          meshName_,
          databases_[procI].timeName(),
          databases_[procI]
        }
      }
    );
    pointProcAddressing_.set
    (
      procI,
      new labelIOList
      {
        IOobject
        {
          "pointProcAddressing",
          meshes_[procI].facesInstance(),
          meshes_[procI].meshSubDir,
          meshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        }
      }
    );
    faceProcAddressing_.set
    (
      procI,
      new labelIOList
      {
        IOobject
        {
          "faceProcAddressing",
          meshes_[procI].facesInstance(),
          meshes_[procI].meshSubDir,
          meshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        }
      }
    );
    cellProcAddressing_.set
    (
      procI,
      new labelIOList
      {
        IOobject
        {
          "cellProcAddressing",
          meshes_[procI].facesInstance(),
          meshes_[procI].meshSubDir,
          meshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        }
      }
    );
    boundaryProcAddressing_.set
    (
      procI,
      new labelIOList
      {
        IOobject
        {
          "boundaryProcAddressing",
          meshes_[procI].facesInstance(),
          meshes_[procI].meshSubDir,
          meshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        }
      }
    );
  }
}


// Constructors 
mousse::processorMeshes::processorMeshes
(
  PtrList<Time>& databases,
  const word& meshName
)
:
  meshName_{meshName},
  databases_{databases},
  meshes_{databases.size()},
  pointProcAddressing_{databases.size()},
  faceProcAddressing_{databases.size()},
  cellProcAddressing_{databases.size()},
  boundaryProcAddressing_{databases.size()}
{
  read();
}


// Member Functions 
mousse::fvMesh::readUpdateState mousse::processorMeshes::readUpdate()
{
  fvMesh::readUpdateState stat = fvMesh::UNCHANGED;
  FOR_ALL(databases_, procI) {
    // Check if any new meshes need to be read.
    fvMesh::readUpdateState procStat = meshes_[procI].readUpdate();
    // Combine into overall mesh change status
    if (stat == fvMesh::UNCHANGED) {
      stat = procStat;
    } else if (stat != procStat) {
      FATAL_ERROR_IN("processorMeshes::readUpdate()")
        << "Processor " << procI
        << " has a different polyMesh at time "
        << databases_[procI].timeName()
        << " compared to any previous processors." << nl
        << "Please check time " << databases_[procI].timeName()
        << " directories on all processors for consistent"
        << " mesh files."
        << exit(FatalError);
    }
  }
  if (stat == fvMesh::TOPO_CHANGE || stat == fvMesh::TOPO_PATCH_CHANGE) {
    // Reread all meshes and addresssing
    read();
  }
  return stat;
}


void mousse::processorMeshes::reconstructPoints(fvMesh& mesh)
{
  // Read the field for all the processors
  PtrList<pointIOField> procsPoints{meshes_.size()};
  FOR_ALL(meshes_, procI) {
    procsPoints.set
    (
      procI,
      new pointIOField
      {
        IOobject
        {
          "points",
          meshes_[procI].time().timeName(),
          polyMesh::meshSubDir,
          meshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        }
      }
    );
  }
  // Create the new points
  vectorField newPoints{mesh.nPoints()};
  FOR_ALL(meshes_, procI) {
    const vectorField& procPoints = procsPoints[procI];
    // Set the cell values in the reconstructed field
    const labelList& pointProcAddressingI = pointProcAddressing_[procI];
    if (pointProcAddressingI.size() != procPoints.size()) {
      FATAL_ERROR_IN("processorMeshes")
        << "problem :"
        << " pointProcAddressingI:" << pointProcAddressingI.size()
        << " procPoints:" << procPoints.size()
        << abort(FatalError);
    }
    FOR_ALL(pointProcAddressingI, pointI) {
      newPoints[pointProcAddressingI[pointI]] = procPoints[pointI];
    }
  }
  mesh.movePoints(newPoints);
  mesh.write();
}

