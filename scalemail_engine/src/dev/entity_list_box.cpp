#include "dev/entity_list_box.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
bool EntityListBox::listItemVectorGetter(void* data, int n, const char** out_text) {
	const auto& v = *static_cast<std::vector<EntityListBox::ListItem>*>(data);
	*out_text = v[n].data->idString.c_str();
	return true;
}

//  ============================================================================
EntityListBox::EntityListBox() : mSelectedIndex(-1) {
}

//  ============================================================================
void EntityListBox::draw() {
	ImGui::PushItemWidth(100);
	ImGui::ListBox(
		"",
		&mSelectedIndex,
		listItemVectorGetter,
		static_cast<void*>(&mListItems),
		mListItems.size());
	ImGui::PopItemWidth();
}

//  ============================================================================
Entity EntityListBox::getSelectedEntity() const {
	if (!this->hasSelectedEntity()) {
		throw std::runtime_error("No entity selected.");
	}

	return mListItems[mSelectedIndex].data->entity;
}

//  ============================================================================
bool EntityListBox::hasSelectedEntity() const {
	return mSelectedIndex >= 0 && mSelectedIndex < mListItems.size();
}

//  ============================================================================
void EntityListBox::populate(const std::vector<DebugComponentData>& debugCmpntData) {
	//	ImGui requires non-const void pointer so cast const away...
	auto& debugData = const_cast<std::vector<DebugComponentData>&>(debugCmpntData);

	unsigned lastSelectedEntityId = 0;
	if (mSelectedIndex != -1) {
		//	Save selected entity ID for later comparison
		lastSelectedEntityId = mListItems[mSelectedIndex].entityId;
	}

	mListItems.clear();
	mListItems.reserve(debugData.size());

	for (auto& data : debugData) {
		ListItem listItem = {};
		listItem.data = &data;
		listItem.entityId = data.entity.id;

		mListItems.push_back(listItem);
	}

	std::sort(
		mListItems.begin(),
		mListItems.end(),
		[](auto& a, auto& b) {
			return a.entityId < b.entityId;
		}
	);

	//	Update selected index
	if (mSelectedIndex != -1) {
		bool found = false;

		const size_t count = mListItems.size();
		for (size_t e = 0; e < count; ++e) {
			//	Use saved entity ID for compare in case component was removed
			if (mListItems[e].entityId == lastSelectedEntityId) {
				mSelectedIndex = e;
				found = true;
				break;
			}
		}

		//	Clear listbox selection if selected entity no longer exists
		if (!found) {
			mSelectedIndex = -1;
		}
	}
}
}
