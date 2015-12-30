// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshReaders::STARCD
// Description
//   Read pro-STAR vrt/cel/bnd files.
//   The protected data in meshReader are filled.
//   Starting with pro-STAR version 4, the files have become easier to read.
//   - vertices are space-delimited.
//   - the cell format is logical.
//   - trimmed and degenerate cells are saved as polyhedral.
//   - the boundaries corresponds to cells and their faces.
// SourceFiles
//   starcd_mesh_reader.cpp
#ifndef starcd_mesh_reader_hpp_
#define starcd_mesh_reader_hpp_
# include "mesh_reader.hpp"
# include "boundary_region.hpp"
# include "cell_shape.hpp"
# include "ifstream.hpp"
namespace mousse
{
namespace meshReaders
{
class STARCD
:
  public meshReader
{
protected:
  // Protected Data
    static const char* const defaultBoundaryName;
    static const char* const defaultSolidBoundaryName;
    //- Face addressing from pro-STAR faces -> OpenFOAM faces
    static const int starToFoamFaceAddr[4][6];
    //- Cell shapes
    cellShapeList cellShapes_;
    //- Point labels (imported Point numbering not necessarily contiguous)
    labelList mapToFoamPointId_;
    //- Cell labels (imported Cell numbering not necessarily contiguous)
    labelList mapToFoamCellId_;
    //- Boundary region data
    boundaryRegion boundaryRegion_;
  // Protected Member Functions
    //- Disallow default bitwise copy construct
    STARCD(const STARCD&);
    //- Disallow default bitwise assignment
    void operator=(const STARCD&);
    //- Read the mesh from the file(s)
    virtual bool readGeometry(const scalar scaleFactor = 1.0);
    //- Read points from file
    void readPoints(const fileName&, const scalar scaleFactor);
    //- Read cell connectivities from file
    virtual void readCells(const fileName&);
    //- Remove unused points
    void cullPoints();
    //- Read boundary (cell/face) definitions
    void readBoundary(const fileName&);
    //- Read auxiliary data from constant/{boundaryRegion,cellTable}
    void readAux(const objectRegistry&);
    //- Read and discard to newline
    static void readToNewline(IFstream&);
    //- Read header
    static bool readHeader(IFstream&, word fileSignature);
protected:
    enum cellType
    {
      starcdFluidType   = 1,
      starcdSolidType   = 2,
      starcdBaffleType  = 3,
      starcdShellType   = 4,
      starcdLineType    = 5,
      starcdPointType   = 6
    };
    enum shapeType
    {
      starcdPoint = 1,
      starcdLine  = 2,
      starcdShell = 3,
      starcdHex   = 11,
      starcdPrism = 12,
      starcdTet   = 13,
      starcdPyr   = 14,
      starcdPoly  = 255
    };
public:
  // Static data
    //- Keep solids (default false)
    static bool keepSolids;
  // Constructors
    //- Construct from case name
    STARCD
    (
      const fileName& prefix,
      const objectRegistry&,
      const scalar scaleFactor = 1.0
    );
  //- Destructor
  virtual ~STARCD();
};
}  // namespace meshReaders
}  // namespace mousse
#endif
