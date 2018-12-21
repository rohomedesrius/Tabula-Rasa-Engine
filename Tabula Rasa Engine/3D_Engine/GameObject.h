#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "trDefs.h"

#include "Component.h"
#include "ComponentTransform.h"

#include "MathGeoLib/MathGeoLib.h"
#include "ParsonJson/parson.h"

#include <string>
#include <list>
#include <map>

class GameObject
{
public:
	GameObject();
	GameObject(const char* name, GameObject* parent);
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);

	bool Save(JSON_Array* array)const;
	bool Load(JSON_Object* go_obj, std::map<GameObject*, UID>& uuid_relations);

	Component* CreateComponent(Component::component_type type);
	Component* CreateComponent(Component::component_type type, Component* component);

	// finds and returns the first component of the type sended
	Component* FindComponentByType(Component::component_type type);

	// getters/setters stuff
	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent, bool force_transform_calc = false);
	const char* GetName() const;
	void SetName(const char* name);
	ComponentTransform* GetTransform()const;
	UID GetUUID()const;

	void RecalculateBoundingBox();

	bool IsActive()const;

	void DestroyGameObjectsIfNeeded();

	GameObject* FindGOByName(const char* name);

private:

	bool active = false;
	std::string name;
	GameObject* parent = nullptr;
	ComponentTransform* transform = nullptr; //Always should have one
	UID uuid = 0u;
	
public:

	AABB bounding_box;

	std::list<GameObject*> childs;
	std::list<Component*> components;

	bool to_destroy = false;
	bool is_static = false;

	bool in_camera = false;
	bool is_active = true;

};

#endif // __GAMEOBJECT_H__