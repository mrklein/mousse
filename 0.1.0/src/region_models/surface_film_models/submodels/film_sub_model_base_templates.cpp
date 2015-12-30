// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
template<class FilmType>
const FilmType& filmSubModelBase::filmType() const
{
  if (!isA<FilmType>(owner_))
  {
    FatalErrorIn("FilmType& surfaceFilmModel::film() const")
      << "Model " << this->modelType() << " requested film type "
      << FilmType::typeName << " but film is type " << owner_.type()
      << abort(FatalError);
  }
  return refCast<const FilmType>(owner_);
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
