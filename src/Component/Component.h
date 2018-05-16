#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
};

#endif //COMPONENT_H
