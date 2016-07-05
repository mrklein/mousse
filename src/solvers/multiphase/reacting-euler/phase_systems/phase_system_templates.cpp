// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blended_interfacial_model.hpp"
// Protected Member Functions 
template<class modelType>
void mousse::phaseSystem::createSubModels
(
  const dictTable& modelDicts,
  HashTable
  <
    autoPtr<modelType>,
    phasePairKey,
    phasePairKey::hash
  >& models
)
{
  FOR_ALL_CONST_ITER(dictTable, modelDicts, iter)
  {
    const phasePairKey& key = iter.key();
    models.insert
    (
      key,
      modelType::New
      (
       *iter,
        phasePairs_[key]
      )
    );
  }
}
template<class modelType>
void mousse::phaseSystem::generatePairsAndSubModels
(
  const word& modelName,
  HashTable
  <
    autoPtr<modelType>,
    phasePairKey,
    phasePairKey::hash
  >& models
)
{
  dictTable modelDicts{lookup(modelName)};
  generatePairs(modelDicts);
  createSubModels(modelDicts, models);
}
template<class modelType>
void mousse::phaseSystem::generatePairsAndSubModels
(
  const word& modelName,
  HashTable
  <
    autoPtr<BlendedInterfacialModel<modelType>>,
    phasePairKey,
    phasePairKey::hash
  >& models
)
{
  typedef
    HashTable<autoPtr<modelType>, phasePairKey, phasePairKey::hash>
    modelTypeTable;
  modelTypeTable tempModels;
  generatePairsAndSubModels(modelName, tempModels);
  const blendingMethod& blending = 
    blendingMethods_.found(modelName)
    ? blendingMethods_[modelName]
    : blendingMethods_["default"];
  autoPtr<modelType> noModel{NULL};
  FOR_ALL_CONST_ITER(typename modelTypeTable, tempModels, iter)
  {
    if (!iter().valid())
    {
      continue;
    }
    const phasePairKey key{iter.key().first(), iter.key().second()};
    const phasePairKey key1In2{key.first(), key.second(), true};
    const phasePairKey key2In1{key.second(), key.first(), true};
    models.insert
    (
      key,
      autoPtr<BlendedInterfacialModel<modelType>>
      {
        new BlendedInterfacialModel<modelType>
        {
          phaseModels_[key.first()],
          phaseModels_[key.second()],
          blending,
          tempModels.found(key    ) ? tempModels[key    ] : noModel,
          tempModels.found(key1In2) ? tempModels[key1In2] : noModel,
          tempModels.found(key2In1) ? tempModels[key2In1] : noModel
        }
      }
    );
  }
}
template<class modelType>
void mousse::phaseSystem::generatePairsAndSubModels
(
  const word& modelName,
  HashTable
  <
    HashTable<autoPtr<modelType>>,
    phasePairKey,
    phasePairKey::hash
  >& models
)
{
  typedef
    HashTable<autoPtr<modelType>, phasePairKey, phasePairKey::hash>
    modelTypeTable;
  FOR_ALL(phaseModels_, phasei)
  {
    modelTypeTable tempModels;
    generatePairsAndSubModels
    (
      IOobject::groupName(modelName, phaseModels_[phasei].name()),
      tempModels
    );
    FOR_ALL_CONST_ITER(typename modelTypeTable, tempModels, tempModelIter)
    {
      const phasePairKey key(tempModelIter.key());
      if (!models.found(key))
      {
        models.insert
        (
          key,
          HashTable<autoPtr<modelType>>()
        );
      }
      models[tempModelIter.key()].insert
      (
        phaseModels_[phasei].name(),
        *tempModelIter
      );
    }
  }
}
template <class modelType>
const modelType& mousse::phaseSystem::lookupSubModel(const phasePair& key) const
{
  return
    mesh().lookupObject<modelType>
    (
      IOobject::groupName(modelType::typeName, key.name())
    );
}
template <class modelType>
const modelType& mousse::phaseSystem::lookupSubModel
(
  const phaseModel& dispersed,
  const phaseModel& continuous
) const
{
  return lookupSubModel<modelType>(orderedPhasePair(dispersed, continuous));
}
