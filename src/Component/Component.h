#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory> // for shared ptr
class GameObject;

class Component {
public:
	// These can be overriden in child classes
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};

protected:
	GameObject* mGameObject;
};

typedef std::shared_ptr<Component> ComponentPtr;

#endif //COMPONENT_H
