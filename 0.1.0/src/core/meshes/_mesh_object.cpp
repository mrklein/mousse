// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_mesh_object.hpp"
#include "object_registry.hpp"
#include "iostreams.hpp"

// Constructors 
template<class Mesh, template<class> class MeshObjectType, class Type>
mousse::MeshObject<Mesh, MeshObjectType, Type>::MeshObject(const Mesh& mesh)
:
  MeshObjectType<Mesh>{Type::typeName, mesh.thisDb()},
  mesh_{mesh}
{}


// Selectors
template<class Mesh, template<class> class MeshObjectType, class Type>
const Type& mousse::MeshObject<Mesh, MeshObjectType, Type>::New
(
  const Mesh& mesh
)
{
  if (mesh.thisDb().objectRegistry::template foundObject<Type>
      (
        Type::typeName
      ))
  {
    return mesh.thisDb().objectRegistry::template lookupObject<Type>
    (
      Type::typeName
    );
  }
  else
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::New(const " << Mesh::typeName
        << "&) : constructing " << Type::typeName
        << " for region " << mesh.name() << endl;
    }
    Type* objectPtr = new Type{mesh};
    regIOobject::store(static_cast<MeshObjectType<Mesh>*>(objectPtr));
    return *objectPtr;
  }
}


template<class Mesh, template<class> class MeshObjectType, class Type>
template<class Data1>
const Type& mousse::MeshObject<Mesh, MeshObjectType, Type>::New
(
  const Mesh& mesh,
  const Data1& d
)
{
  if
  (
    mesh.thisDb().objectRegistry::template foundObject<Type>
    (
      Type::typeName
    )
  )
  {
    return mesh.thisDb().objectRegistry::template lookupObject<Type>
    (
      Type::typeName
    );
  }
  else
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::New(const " << Mesh::typeName
        << "&, const Data1&) : constructing " << Type::typeName
        << " for region " << mesh.name() << endl;
    }
    Type* objectPtr = new Type{mesh, d};
    regIOobject::store(static_cast<MeshObjectType<Mesh>*>(objectPtr));
    return *objectPtr;
  }
}


template<class Mesh, template<class> class MeshObjectType, class Type>
template<class Data1, class Data2>
const Type& mousse::MeshObject<Mesh, MeshObjectType, Type>::New
(
  const Mesh& mesh,
  const Data1& d1,
  const Data2& d2
)
{
  if
  (
    mesh.thisDb().objectRegistry::template foundObject<Type>
    (
      Type::typeName
    )
  )
  {
    return mesh.thisDb().objectRegistry::template lookupObject<Type>
    (
      Type::typeName
    );
  }
  else
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::New(const " << Mesh::typeName
        << "&, const Data[1-2]&) : constructing " << Type::typeName
        << " for region " << mesh.name() << endl;
    }
    Type* objectPtr = new Type{mesh, d1, d2};
    // Make sure to register the top level regIOobject for if Type itself
    // is a regIOobject
    regIOobject::store(static_cast<MeshObjectType<Mesh>*>(objectPtr));
    return *objectPtr;
  }
}


template<class Mesh, template<class> class MeshObjectType, class Type>
template<class Data1, class Data2, class Data3>
const Type& mousse::MeshObject<Mesh, MeshObjectType, Type>::New
(
  const Mesh& mesh,
  const Data1& d1,
  const Data2& d2,
  const Data3& d3
)
{
  if
  (
    mesh.thisDb().objectRegistry::template foundObject<Type>
    (
      Type::typeName
    )
  )
  {
    return mesh.thisDb().objectRegistry::template lookupObject<Type>
    (
      Type::typeName
    );
  }
  else
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::New(const " << Mesh::typeName
        << "&, const Data[1-3]&) : constructing " << Type::typeName
        << " for region " << mesh.name() << endl;
    }
    Type* objectPtr = new Type{mesh, d1, d2, d3};
    regIOobject::store(static_cast<MeshObjectType<Mesh>*>(objectPtr));
    return *objectPtr;
  }
}


template<class Mesh, template<class> class MeshObjectType, class Type>
template<class Data1, class Data2, class Data3, class Data4>
const Type& mousse::MeshObject<Mesh, MeshObjectType, Type>::New
(
  const Mesh& mesh,
  const Data1& d1,
  const Data2& d2,
  const Data3& d3,
  const Data4& d4
)
{
  if
  (
    mesh.thisDb().objectRegistry::template foundObject<Type>
    (
      Type::typeName
    )
  )
  {
    return mesh.thisDb().objectRegistry::template lookupObject<Type>
    (
      Type::typeName
    );
  }
  else
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::New(const " << Mesh::typeName
        << "&, const Data[1-4]&) : constructing " << Type::typeName
        << " for region " << mesh.name() << endl;
    }
    Type* objectPtr = new Type{mesh, d1, d2, d3, d4};
    regIOobject::store(static_cast<MeshObjectType<Mesh>*>(objectPtr));
    return *objectPtr;
  }
}


// Destructor 
template<class Mesh, template<class> class MeshObjectType, class Type>
bool mousse::MeshObject<Mesh, MeshObjectType, Type>::Delete(const Mesh& mesh)
{
  if
  (
    mesh.thisDb().objectRegistry::template foundObject<Type>
    (
      Type::typeName
    )
  )
  {
    if (meshObject::debug)
    {
      Pout<< "MeshObject::Delete(const Mesh&) : deleting "
        << Type::typeName << endl;
    }
    return mesh.thisDb().checkOut
    (
      const_cast<Type&>
      (
        mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
          Type::typeName
        )
      )
    );
  }
  else
  {
    return false;
  }
}


template<class Mesh, template<class> class MeshObjectType, class Type>
mousse::MeshObject<Mesh, MeshObjectType, Type>::~MeshObject()
{
  MeshObjectType<Mesh>::release();
}


template<class Mesh>
void mousse::meshObject::movePoints(objectRegistry& obr)
{
  HashTable<GeometricMeshObject<Mesh>*> meshObjects
  {
    obr.lookupClass<GeometricMeshObject<Mesh>>()
  };
  if (meshObject::debug)
  {
    Pout<< "meshObject::movePoints(objectRegistry&) :"
      << " moving " << Mesh::typeName
      << " meshObjects for region " << obr.name() << endl;
  }
  FOR_ALL_ITER
  (
    typename HashTable<GeometricMeshObject<Mesh>*>,
    meshObjects,
    iter
  )
  {
    if (isA<MoveableMeshObject<Mesh> >(*iter()))
    {
      if (meshObject::debug)
      {
        Pout<< "    Moving " << iter()->name() << endl;
      }
      dynamic_cast<MoveableMeshObject<Mesh>*>(iter())->movePoints();
    }
    else
    {
      if (meshObject::debug)
      {
        Pout<< "    Destroying " << iter()->name() << endl;
      }
      obr.checkOut(*iter());
    }
  }
}


template<class Mesh>
void mousse::meshObject::updateMesh(objectRegistry& obr, const mapPolyMesh& mpm)
{
  HashTable<GeometricMeshObject<Mesh>*> meshObjects
  {
    obr.lookupClass<GeometricMeshObject<Mesh>>()
  };
  if (meshObject::debug)
  {
    Pout<< "meshObject::updateMesh(objectRegistry&, "
       "const mapPolyMesh& mpm) : updating " << Mesh::typeName
      << " meshObjects for region " << obr.name() << endl;
  }
  FOR_ALL_ITER
  (
    typename HashTable<GeometricMeshObject<Mesh>*>,
    meshObjects,
    iter
  )
  {
    if (isA<UpdateableMeshObject<Mesh> >(*iter()))
    {
      if (meshObject::debug)
      {
        Pout<< "    Updating " << iter()->name() << endl;
      }
      dynamic_cast<UpdateableMeshObject<Mesh>*>(iter())->updateMesh(mpm);
    }
    else
    {
      if (meshObject::debug)
      {
        Pout<< "    Destroying " << iter()->name() << endl;
      }
      obr.checkOut(*iter());
    }
  }
}


template<class Mesh, template<class> class MeshObjectType>
void mousse::meshObject::clear(objectRegistry& obr)
{
  HashTable<MeshObjectType<Mesh>*> meshObjects
  {
    obr.lookupClass<MeshObjectType<Mesh> >()
  };
  if (meshObject::debug)
  {
    Pout<< "meshObject::clear(objectRegistry&) :"
      << " clearing " << Mesh::typeName
      << " meshObjects for region " << obr.name() << endl;
  }
  FOR_ALL_ITER(typename HashTable<MeshObjectType<Mesh>*>, meshObjects, iter)
  {
    if (meshObject::debug)
    {
      Pout<< "    Destroying " << iter()->name() << endl;
    }
    obr.checkOut(*iter());
  }
}


template
<
  class Mesh,
  template<class> class FromType,
  template<class> class ToType
>
void mousse::meshObject::clearUpto(objectRegistry& obr)
{
  HashTable<FromType<Mesh>*> meshObjects
  {
    obr.lookupClass<FromType<Mesh> >()
  };
  if (meshObject::debug)
  {
    Pout<< "meshObject::clearUpto(objectRegistry&) :"
      << " clearing " << Mesh::typeName
      << " meshObjects for region " << obr.name() << endl;
  }
  FOR_ALL_ITER(typename HashTable<FromType<Mesh>*>, meshObjects, iter)
  {
    if (!isA<ToType<Mesh> >(*iter()))
    {
      if (meshObject::debug)
      {
        Pout<< "    Destroying " << iter()->name() << endl;
      }
      obr.checkOut(*iter());
    }
  }
}
