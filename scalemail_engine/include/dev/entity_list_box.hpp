#pragma once

#include "dev/debug_system.hpp"

namespace ScaleMail
{
class EntityListBox
{
	struct ListItem
	{
		unsigned entityId;
		DebugComponentData* data;
	};

	int mSelectedIndex;
	std::vector<ListItem> mListItems;

	static bool listItemVectorGetter(void* data, int n, const char** out_text);

public:
	EntityListBox();
	void draw();
	Entity getSelectedEntity() const;
	bool hasSelectedEntity() const;
	void populate(const std::vector<DebugComponentData>& debugCmpntData);
};
}
