#pragma once

#include "dev/debug_system.hpp"

namespace ScaleMail
{
class EntityListBox
{
	int mSelectedIndex;
	std::vector<DebugComponentData> mEntities;

public:
	EntityListBox();
	void draw();
	Entity getSelectedEntity() const;
	bool hasSelectedEntity() const;
	void setEntities(std::vector<DebugComponentData>& entities);
};
}
