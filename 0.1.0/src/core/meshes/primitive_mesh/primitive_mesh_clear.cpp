// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "demand_driven_data.hpp"
// Member Functions 
void mousse::primitiveMesh::printAllocated() const
{
  Pout<< "primitiveMesh allocated :" << endl;
  // Topology
  if (cellShapesPtr_)
  {
    Pout<< "    Cell shapes" << endl;
  }
  if (edgesPtr_)
  {
    Pout<< "    Edges" << endl;
  }
  if (ccPtr_)
  {
    Pout<< "    Cell-cells" << endl;
  }
  if (ecPtr_)
  {
    Pout<< "    Edge-cells" << endl;
  }
  if (pcPtr_)
  {
    Pout<< "    Point-cells" << endl;
  }
  if (cfPtr_)
  {
    Pout<< "    Cell-faces" << endl;
  }
  if (efPtr_)
  {
    Pout<< "    Edge-faces" << endl;
  }
  if (pfPtr_)
  {
    Pout<< "    Point-faces" << endl;
  }
  if (cePtr_)
  {
    Pout<< "    Cell-edges" << endl;
  }
  if (fePtr_)
  {
    Pout<< "    Face-edges" << endl;
  }
  if (pePtr_)
  {
    Pout<< "    Point-edges" << endl;
  }
  if (ppPtr_)
  {
    Pout<< "    Point-point" << endl;
  }
  if (cpPtr_)
  {
    Pout<< "    Cell-point" << endl;
  }
  // Geometry
  if (cellCentresPtr_)
  {
    Pout<< "    Cell-centres" << endl;
  }
  if (faceCentresPtr_)
  {
    Pout<< "    Face-centres" << endl;
  }
  if (cellVolumesPtr_)
  {
    Pout<< "    Cell-volumes" << endl;
  }
  if (faceAreasPtr_)
  {
    Pout<< "    Face-areas" << endl;
  }
}
void mousse::primitiveMesh::clearGeom()
{
  if (debug)
  {
    Pout<< "primitiveMesh::clearGeom() : "
      << "clearing geometric data"
      << endl;
  }
  deleteDemandDrivenData(cellCentresPtr_);
  deleteDemandDrivenData(faceCentresPtr_);
  deleteDemandDrivenData(cellVolumesPtr_);
  deleteDemandDrivenData(faceAreasPtr_);
}
void mousse::primitiveMesh::clearAddressing()
{
  if (debug)
  {
    Pout<< "primitiveMesh::clearAddressing() : "
      << "clearing topology"
      << endl;
  }
  deleteDemandDrivenData(cellShapesPtr_);
  clearOutEdges();
  deleteDemandDrivenData(ccPtr_);
  deleteDemandDrivenData(ecPtr_);
  deleteDemandDrivenData(pcPtr_);
  deleteDemandDrivenData(cfPtr_);
  deleteDemandDrivenData(efPtr_);
  deleteDemandDrivenData(pfPtr_);
  deleteDemandDrivenData(cePtr_);
  deleteDemandDrivenData(fePtr_);
  deleteDemandDrivenData(pePtr_);
  deleteDemandDrivenData(ppPtr_);
  deleteDemandDrivenData(cpPtr_);
}
void mousse::primitiveMesh::clearOut()
{
  clearGeom();
  clearAddressing();
}
