#ifndef CORE_FIELDS_FIELDS_TRANSFORM_LIST_HPP_
#define CORE_FIELDS_FIELDS_TRANSFORM_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform.hpp"
#include "list.hpp"
#include "map.hpp"
#include "edge_map.hpp"
#include "tensor_field.hpp"


namespace mousse {

//- Extend transform to work on list.
template<class T>
List<T> transform
(
  const tensor& rotTensor,
  const UList<T>& field
);

//- Apply transformation to list. Either single transformation tensor
//  or one tensor per element.
template<class T>
void transformList(const tensor&, UList<T>&);

template<class T>
void transformList(const tensorField&, UList<T>&);

template<class T>
void transformList(const tensor&, Map<T>&);

template<class T>
void transformList(const tensorField&, Map<T>&);

template<class T>
void transformList(const tensor&, EdgeMap<T>&);

template<class T>
void transformList(const tensorField&, EdgeMap<T>&);

// Specialisations for bool
template<>
inline void transformList(const tensor&, UList<bool>&)
{}

template<>
inline void transformList(const tensorField&, UList<bool>&)
{}

template<>
inline void transformList(const tensor&, Map<bool>&)
{}

template<>
inline void transformList(const tensorField&, Map<bool>&)
{}

template<>
inline void transformList(const tensor&, EdgeMap<bool>&)
{}

template<>
inline void transformList(const tensorField&, EdgeMap<bool>&)
{}

// Specialisations for label
template<>
inline void transformList(const tensor&, labelUList&)
{}

template<>
inline void transformList(const tensorField&, labelUList&)
{}

template<>
inline void transformList(const tensor&, Map<label>&)
{}

template<>
inline void transformList(const tensorField&, Map<label>&)
{}

template<>
inline void transformList(const tensor&, EdgeMap<label>&)
{}

template<>
inline void transformList(const tensorField&, EdgeMap<label>&)
{}

// Specialisations for scalar
template<>
inline void transformList(const tensor&, UList<scalar>&)
{}

template<>
inline void transformList(const tensorField&, UList<scalar>&)
{}

template<>
inline void transformList(const tensor&, Map<scalar>&)
{}

template<>
inline void transformList(const tensorField&, Map<scalar>&)
{}

template<>
inline void transformList(const tensor&, EdgeMap<scalar>&)
{}

template<>
inline void transformList(const tensorField&, EdgeMap<scalar>&)
{}

}  // namespace mousse

#include "transform_list.ipp"

#endif
