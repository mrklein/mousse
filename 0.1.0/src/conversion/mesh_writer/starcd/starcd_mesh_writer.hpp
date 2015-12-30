// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshWriters::STARCD
// Description
//   Writes polyMesh in pro-STAR (v4) bnd/cel/vrt format
//   The cellTableId and cellTable information are used (if available).
//   Otherwise the cellZones are used (if available).
// SourceFiles
//   starcd_mesh_writer.cpp
#ifndef starcd_mesh_writer_hpp_
#define starcd_mesh_writer_hpp_
#include "mesh_writer.hpp"
#include "iostream.hpp"
namespace mousse
{
namespace meshWriters
{
class STARCD
:
  public meshWriter
{
  // Private Data
    static const char* defaultBoundaryName;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    STARCD(const STARCD&);
    //- Disallow default bitwise assignment
    void operator=(const STARCD&);
    //- Pro-STAR 4+ header format
    static void writeHeader(Ostream&, const char* filetype);
    //- Write points
    void writePoints(const fileName& baseName) const;
    //- Write cells
    void writeCells(const fileName& baseName) const;
    //- Write boundary
    void writeBoundary(const fileName& baseName) const;
    void getCellTable();
    label findDefaultBoundary() const;
public:
  // Static data members
    //- Face addressing from OpenFOAM faces -> pro-STAR faces
    static const label foamToStarFaceAddr[4][6];
  // Constructors
    //- Open a file for writing
    STARCD
    (
      const polyMesh&,
      const scalar scaleFactor = 1.0
    );
  //- Destructor
  virtual ~STARCD();
  // Member Functions
    // Edit
      //- Remove STAR-CD files for the baseName
      void rmFiles(const fileName& baseName) const;
    // Write
      //- Write volume mesh
      virtual bool write
      (
        const fileName& meshName = fileName::null
      ) const;
};
}  // namespace meshWriters
}  // namespace mousse
#endif
