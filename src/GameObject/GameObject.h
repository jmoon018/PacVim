#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory> // for shared ptr
#include "../Component/Component.h"

// A GameObject is an entity that stores components; the sum of the
// components creates the behavior for the particular GameObject
// Generally should be used for 'physical' entities (like ghosts, player, walls, etc)
//    as opposed to more abstract entities (managers, etc)
class GameObject {
public:

	void AddComponent(Component);

	void Start();
	void Update();
	void LateUpdate();
private:
	std::vector<ComponentPtr> mComponents;

};

typedef std::shared_ptr<GameObject> GameObjectPtr;

#endif //GAMEOBJECT_H
