// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::tabulatedHeatTransfer
// Description
//   Tabulated heat transfer model. The heat exchange area per unit volume
//   must be provided.  The 2D table returns the heat transfer coefficient
//   by querying the local and neighbour region velocities
#ifndef tabulated_heat_transfer_hpp_
#define tabulated_heat_transfer_hpp_
#include "inter_region_heat_transfer_model.hpp"
#include "auto_ptr.hpp"
#include "interpolation_2d_table.hpp"
namespace mousse
{
namespace fv
{
class tabulatedHeatTransfer
:
  public interRegionHeatTransferModel
{
private:
  // Private data
    //- Name of velocity field; default = U
    word UName_;
    //- Name of neighbour velocity field; default = U
    word UNbrName_;
    //- 2D look up table
    autoPtr<interpolation2DTable<scalar> > hTable_;
    //- Area per unit volume of heat exchanger
    autoPtr<volScalarField> AoV_;
    //- Heat transfer coefficient table
    const interpolation2DTable<scalar>& hTable();
    //- Field of area divided by volume
    const volScalarField& AoV();
    //- Start time name
    const word startTimeName_;
public:
  //- Runtime type information
    TypeName("tabulatedHeatTransfer");
  // Constructors
    //- Construct from dictionary
    tabulatedHeatTransfer
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~tabulatedHeatTransfer();
  // Public Functions
    //- Calculate the heat transfer coefficient
    virtual void calculateHtc();
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
