// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gidaspow_ergun_wen_yu.hpp"
#include "phase_pair.hpp"
#include "ergun.hpp"
#include "wen_yu.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(GidaspowErgunWenYu, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, GidaspowErgunWenYu, dictionary); 
}
}
// Constructors 
mousse::dragModels::GidaspowErgunWenYu::GidaspowErgunWenYu
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject},
  Ergun_
  {
    new Ergun
    {
      dict,
      pair,
      false
    }
  },
  WenYu_
  {
    new WenYu
    {
      dict,
      pair,
      false
    }
  }
{}
// Destructor 
mousse::dragModels::GidaspowErgunWenYu::~GidaspowErgunWenYu()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::dragModels::GidaspowErgunWenYu::CdRe() const
{
  return
    pos(pair_.continuous() - 0.8)*WenYu_->CdRe()
    + neg(pair_.continuous() - 0.8)*Ergun_->CdRe();
}
