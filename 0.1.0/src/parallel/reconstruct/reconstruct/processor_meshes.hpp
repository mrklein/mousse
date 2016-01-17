// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorMeshes
// Description
//   Container for processor mesh addressing.
// SourceFiles
//   processor_meshes.cpp
#ifndef processor_meshes_hpp_
#define processor_meshes_hpp_
#include "ptr_list.hpp"
#include "fv_mesh.hpp"
#include "ioobject_list.hpp"
#include "label_io_list.hpp"
namespace mousse
{
class processorMeshes
{
  // Private data
    const word meshName_;
    //- Processor databases
    PtrList<Time>& databases_;
    //- List of processor meshes
    PtrList<fvMesh> meshes_;
    //- List of processor point addressing lists
    PtrList<labelIOList> pointProcAddressing_;
    //- List of processor face addressing lists
    PtrList<labelIOList> faceProcAddressing_;
    //- List of processor cell addressing lists
    PtrList<labelIOList> cellProcAddressing_;
    //- List of processor boundary addressing lists
    PtrList<labelIOList> boundaryProcAddressing_;
  // Private Member Functions
    //- Read all meshes
    void read();
public:
  // Constructors
    //- Construct from components
    processorMeshes(PtrList<Time>& databases, const word& meshName);
    //- Disallow default bitwise copy construct
    processorMeshes(const processorMeshes&) = delete;
    //- Disallow default bitwise assignment
    processorMeshes& operator=(const processorMeshes&) = delete;
  // Member Functions
    //- Update the meshes based on the mesh files saved in time directories
    fvMesh::readUpdateState readUpdate();
    //- Reconstruct point position after motion in parallel
    void reconstructPoints(fvMesh&);
    PtrList<fvMesh>& meshes()
    {
      return meshes_;
    }
    const PtrList<labelIOList>& pointProcAddressing() const
    {
      return pointProcAddressing_;
    }
    PtrList<labelIOList>& faceProcAddressing()
    {
      return faceProcAddressing_;
    }
    const PtrList<labelIOList>& cellProcAddressing() const
    {
      return cellProcAddressing_;
    }
    const PtrList<labelIOList>& boundaryProcAddressing() const
    {
      return boundaryProcAddressing_;
    }
};
}  // namespace mousse
#endif
