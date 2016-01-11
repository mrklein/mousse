// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::viewFactor
// Description
//   View factor radiation model. The system solved is: C q = b
//   where:
//       Cij  = deltaij/Ej - (1/Ej - 1)Fij
//       q    = heat flux
//       b    = A eb - Ho
//   and:
//       eb   = sigma*T^4
//       Ej   = emissivity
//       Aij  = deltaij - Fij
//       Fij  = view factor matrix
// SourceFiles
//   view_factor.cpp
#ifndef radiationModelview_factor_hpp_
#define radiationModelview_factor_hpp_
#include "radiation_model.hpp"
#include "single_cell_fv_mesh.hpp"
#include "scalar_matrices.hpp"
#include "global_index.hpp"
#include "scalar_list_io_list.hpp"
#include "map_distribute.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace radiation
{
class viewFactor
:
  public radiationModel
{
  // Private data
    //- Agglomeration List
    labelListIOList finalAgglom_;
    //- Map distributed
    autoPtr<mapDistribute> map_;
    //- Coarse mesh
    singleCellFvMesh coarseMesh_;
    //- Net radiative heat flux [W/m2]
    volScalarField Qr_;
    //- View factor matrix
    autoPtr<scalarSquareMatrix> Fmatrix_;
    //- Inverse of C matrix
    autoPtr<scalarSquareMatrix> CLU_;
    //- Selected patches
    labelList selectedPatches_;
    //- Total global coarse faces
    label totalNCoarseFaces_;
    //- Total local coarse faces
    label nLocalCoarseFaces_;
    //- Constant emissivity
    bool constEmissivity_;
    //- Iterations Counter
    label iterCounter_;
    //- Pivot Indices for LU decomposition
    labelList pivotIndices_;
  // Private Member Functions
    //- Initialise
    void initialise();
    //- Insert view factors into main matrix
    void insertMatrixElements
    (
      const globalIndex& index,
      const label fromProcI,
      const labelListList& globalFaceFaces,
      const scalarListList& viewFactors,
      scalarSquareMatrix& matrix
    );
public:
  //- Runtime type information
  TYPE_NAME("viewFactor");
  // Constructors
    //- Construct from components
    viewFactor(const volScalarField& T);
    //- Construct from components
    viewFactor(const dictionary& dict, const volScalarField& T);
    //- Disallow default bitwise copy construct
    viewFactor(const viewFactor&) = delete;
    //- Disallow default bitwise assignment
    viewFactor& operator=(const viewFactor&) = delete;
  //- Destructor
  virtual ~viewFactor();
  // Member functions
    // Edit
      //- Solve system of equation(s)
      void calculate();
      //- Read radiation properties dictionary
      bool read();
      //- Source term component (for power of T^4)
      virtual tmp<volScalarField> Rp() const;
      //- Source term component (constant)
      virtual tmp<DimensionedField<scalar, volMesh> > Ru() const;
  // Access
    //- Const access to total radiative heat flux field
    inline const volScalarField& Qr() const;
};

inline const mousse::volScalarField& mousse::radiation::viewFactor::Qr() const
{
  return Qr_;
}
}  // namespace radiation
}  // namespace mousse
#endif
