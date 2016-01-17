// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lagrangian_writer.hpp"
#include "write_funs.hpp"
#include "cloud.hpp"
#include "passive_particle.hpp"
// Constructors 
mousse::lagrangianWriter::lagrangianWriter
(
  const vtkMesh& vMesh,
  const bool binary,
  const fileName& fName,
  const word& cloudName,
  const bool dummyCloud
)
:
  vMesh_{vMesh},
  binary_{binary},
  fName_{fName},
  cloudName_{cloudName},
  os_{fName.c_str()}
{
  const fvMesh& mesh = vMesh_.mesh();
  // Write header
  writeFuns::writeHeader(os_, binary_, mesh.time().caseName());
  os_ << "DATASET POLYDATA" << std::endl;
  if (dummyCloud)
  {
    nParcels_ = 0;
    os_ << "POINTS " << nParcels_ << " float" << std::endl;
  }
  else
  {
    Cloud<passiveParticle> parcels(mesh, cloudName_, false);
    nParcels_ = parcels.size();
    os_ << "POINTS " << nParcels_ << " float" << std::endl;
    DynamicList<floatScalar> partField(3*parcels.size());
    FOR_ALL_CONST_ITER(Cloud<passiveParticle>, parcels, elmnt)
    {
      writeFuns::insert(elmnt().position(), partField);
    }
    writeFuns::write(os_, binary_, partField);
  }
}
// Member Functions 
void mousse::lagrangianWriter::writeParcelHeader(const label nFields)
{
  os_ << "POINT_DATA " << nParcels_ << std::endl
    << "FIELD attributes " << nFields
    << std::endl;
}
