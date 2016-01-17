// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactionRateFlameArea
// Description
//   Abstract class for reaction rate per flame area unit
// SourceFiles
//   reaction_rate_flame_area.cpp
//   reaction_rate_flame_area_new.cpp
#ifndef reaction_rate_flame_area_hpp_
#define reaction_rate_flame_area_hpp_
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "vol_fields.hpp"
#include "combustion_model.hpp"
namespace mousse
{
class fvMesh;
class reactionRateFlameArea
{
protected:
  // Protected data
    //- Dictionary
    dictionary coeffDict_;
    //- Mesh reference
    const fvMesh& mesh_;
    //- Combstion model owner
    const combustionModel& combModel_;
    //- Fuel name
    word fuel_;
    //- Fuel consumption rate per unit of flame area
    volScalarField omega_;
public:
  //- Runtime type information
  TYPE_NAME("reactionRateFlameArea");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      reactionRateFlameArea,
      dictionary,
      (
        const word modelType,
        const dictionary& dict,
        const fvMesh& mesh,
        const combustionModel& combModel
      ),
      (modelType, dict, mesh, combModel)
    );
  // Constructors
    //- Construct from dictionary and psiReactionThermo
    reactionRateFlameArea
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const combustionModel& combModel
    );
    //- Construct from components
    reactionRateFlameArea
    (
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh,
      const combustionModel& combModel
    );
    //- Disallow copy construct
    reactionRateFlameArea(const reactionRateFlameArea&) = delete;
    //- Disallow default bitwise assignment
   reactionRateFlameArea& operator=(const reactionRateFlameArea&) = delete;
  // Selector
    static autoPtr<reactionRateFlameArea> New
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const combustionModel& combModel
    );
  // Destructor
    virtual ~reactionRateFlameArea();
  // Member functions
    //- Access functions
      //- Return omega
      const volScalarField& omega() const
      {
        return omega_;
      }
    //- Correct omega
    virtual void correct(const volScalarField& sigma) = 0;
    //- Update from dictionary
    virtual bool read(const dictionary& dictProperties);
};
}  // namespace mousse
#endif
