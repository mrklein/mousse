// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blended_interfacial_model.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
#include "surface_interpolate.hpp"
// Private Member Functions 
template<class modelType>
template<class GeometricField>
void mousse::BlendedInterfacialModel<modelType>::correctFixedFluxBCs
(
  GeometricField& field
) const
{
  FOR_ALL(pair_.phase1().phi().boundaryField(), patchI)
  {
    if (isA<fixedValueFvsPatchScalarField>
        (
          pair_.phase1().phi().boundaryField()[patchI]
        )
       )
    {
      field.boundaryField()[patchI]
       = pTraits<typename GeometricField::value_type>::zero;
    }
  }
}
// Constructors 
template<class modelType>
mousse::BlendedInterfacialModel<modelType>::BlendedInterfacialModel
(
  const phasePair::dictTable& modelTable,
  const blendingMethod& blending,
  const phasePair& pair,
  const orderedPhasePair& pair1In2,
  const orderedPhasePair& pair2In1,
  const bool correctFixedFluxBCs
)
:
  pair_{pair},
  pair1In2_{pair1In2},
  pair2In1_{pair2In1},
  blending_{blending},
  correctFixedFluxBCs_{correctFixedFluxBCs}
{
  if (modelTable.found(pair_))
  {
    model_.set
    (
      modelType::New
      (
        modelTable[pair_],
        pair_
      ).ptr()
    );
  }
  if (modelTable.found(pair1In2_))
  {
    model1In2_.set
    (
      modelType::New
      (
        modelTable[pair1In2_],
        pair1In2_
      ).ptr()
    );
  }
  if (modelTable.found(pair2In1_))
  {
    model2In1_.set
    (
      modelType::New
      (
        modelTable[pair2In1_],
        pair2In1_
      ).ptr()
    );
  }
}
// Destructor 
template<class modelType>
mousse::BlendedInterfacialModel<modelType>::~BlendedInterfacialModel()
{}
// Member Functions 
template<class modelType>
mousse::tmp<mousse::volScalarField>
mousse::BlendedInterfacialModel<modelType>::K() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  tmp<volScalarField> x
  {
    new volScalarField
    {
      {
        modelType::typeName + ":K",
        pair_.phase1().mesh().time().timeName(),
        pair_.phase1().mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pair_.phase1().mesh(),
      {"zero", modelType::dimK, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->K()*(f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->K()*(1 - f1);
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->K()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class modelType>
mousse::tmp<mousse::surfaceScalarField>
mousse::BlendedInterfacialModel<modelType>::Kf() const
{
  tmp<surfaceScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = fvc::interpolate
    (
      blending_.f1(pair1In2_.dispersed(), pair2In1_.dispersed())
    );
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = fvc::interpolate
    (
      blending_.f2(pair1In2_.dispersed(), pair2In1_.dispersed())
    );
  }
  tmp<surfaceScalarField> x
  {
    new surfaceScalarField
    {
      {
        modelType::typeName + ":Kf",
        pair_.phase1().mesh().time().timeName(),
        pair_.phase1().mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pair_.phase1().mesh(),
      {"zero", modelType::dimK, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->Kf()*(f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->Kf()*(1 - f1);
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->Kf()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class modelType>
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::BlendedInterfacialModel<modelType>::F() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  tmp<GeometricField<Type, fvPatchField, volMesh> > x
  {
    new GeometricField<Type, fvPatchField, volMesh>
    {
      {
        modelType::typeName + ":F",
        pair_.phase1().mesh().time().timeName(),
        pair_.phase1().mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pair_.phase1().mesh(),
      {"zero", modelType::dimF, pTraits<Type>::zero}
    }
  };
  if (model_.valid())
  {
    x() += model_->F()*(f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->F()*(1 - f1);
  }
  if (model2In1_.valid())
  {
    x() -= model2In1_->F()*f2; // note : subtraction
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class modelType>
mousse::tmp<mousse::surfaceScalarField>
mousse::BlendedInterfacialModel<modelType>::Ff() const
{
  tmp<surfaceScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = fvc::interpolate
    (
      blending_.f1(pair1In2_.dispersed(), pair2In1_.dispersed())
    );
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = fvc::interpolate
    (
      blending_.f2(pair1In2_.dispersed(), pair2In1_.dispersed())
    );
  }
  tmp<surfaceScalarField> x
  {
    new surfaceScalarField
    {
      {
        modelType::typeName + ":Ff",
        pair_.phase1().mesh().time().timeName(),
        pair_.phase1().mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pair_.phase1().mesh(),
      {"zero", modelType::dimF*dimArea, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->Ff()*(f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->Ff()*(1 - f1);
  }
  if (model2In1_.valid())
  {
    x() -= model2In1_->Ff()*f2; // note : subtraction
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class modelType>
mousse::tmp<mousse::volScalarField>
mousse::BlendedInterfacialModel<modelType>::D() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(pair1In2_.dispersed(), pair2In1_.dispersed());
  }
  tmp<volScalarField> x
  {
    new volScalarField
    {
      {
        modelType::typeName + ":D",
        pair_.phase1().mesh().time().timeName(),
        pair_.phase1().mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pair_.phase1().mesh(),
      {"zero", modelType::dimD, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->D()*(f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->D()*(1 - f1);
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->D()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class modelType>
bool mousse::BlendedInterfacialModel<modelType>::hasModel
(
  const class phaseModel& phase
) const
{
  return &phase == &(pair_.phase1()) ? model1In2_.valid() : model2In1_.valid();
}
template<class modelType>
const modelType& mousse::BlendedInterfacialModel<modelType>::phaseModel
(
  const class phaseModel& phase
) const
{
  return &phase == &(pair_.phase1()) ? model1In2_ : model2In1_;
}
