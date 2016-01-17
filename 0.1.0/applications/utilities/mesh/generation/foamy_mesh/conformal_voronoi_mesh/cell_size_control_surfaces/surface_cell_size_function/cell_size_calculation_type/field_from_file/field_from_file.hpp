// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldFromFile
// Description
// SourceFiles
//   field_from_file.cpp
#ifndef field_from_file_hpp_
#define field_from_file_hpp_
#include "cell_size_calculation_type.hpp"
#include "tri_surface_fields.hpp"
#include "primitive_patch_interpolation.hpp"
namespace mousse
{
class triSurfaceMesh;
class fieldFromFile
:
  public cellSizeCalculationType
{
private:
  // Private data
    //- Dictionary of coefficients for automatic cell sizing
    const dictionary& coeffsDict_;
    //- Name of the triSurfaceScalarField file to load in. Must be in
    //  constant/triSurface
    const word fileName_;
    //- Multiply all the point sizes by this value
    const scalar cellSizeMultipleCoeff_;
public:
  //- Runtime type information
  TypeName("fieldFromFile");
  // Constructors
    //- Construct from components
    fieldFromFile
    (
      const dictionary& cellSizeCalcTypeDict,
      const triSurfaceMesh& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~fieldFromFile()
  {}
  // Member Functions
    //- Load the cell size field
    virtual tmp<triSurfacePointScalarField> load();
};
}  // namespace mousse
#endif
