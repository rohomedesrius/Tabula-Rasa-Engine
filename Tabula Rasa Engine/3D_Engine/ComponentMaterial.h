#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentMaterial : public Component
{

public:

	ComponentMaterial(GameObject* embedded_game_object);
	~ComponentMaterial();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	void SetAmbientColor(float4 ambient_color);
	const float4 GetAmbientColor()const;

	bool SetResource(UID resource);

private:

	float4 ambient_color = float4::zero;

};

#endif // __COMPONENT_MATERIAL_H__