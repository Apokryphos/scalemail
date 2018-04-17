#include "dev/entity_list_box.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
static bool debugDataVectorGetter(void* data, int n, const char** out_text) {
	const auto& v = *static_cast<std::vector<DebugComponentData>*>(data);
	*out_text = v[n].idString.c_str();
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
		debugDataVectorGetter,
		static_cast<void*>(&mEntities),
		mEntities.size());
	ImGui::PopItemWidth();
}

//  ============================================================================
Entity EntityListBox::getSelectedEntity() const {
	if (!this->hasSelectedEntity()) {
		throw std::runtime_error("No entity selected.");
	}

	return mEntities[mSelectedIndex].entity;
}

//  ============================================================================
bool EntityListBox::hasSelectedEntity() const {
	return mSelectedIndex >= 0 && mSelectedIndex < mEntities.size();
}

//  ============================================================================
void EntityListBox::setEntities(std::vector<DebugComponentData>& entities) {
	//	Update selected index when the entities vector changes.
	//	Needed since entity systems swap remove components.
	if (mSelectedIndex != -1) {
		const Entity selectedEntity = mEntities[mSelectedIndex].entity;

		bool found = false;

		for (int e = 0; e < entities.size(); ++e) {
			if (entities[e].entity.id == selectedEntity.id) {
				mSelectedIndex = e;
				found = true;
				break;
			}
		}

		if (!found) {
			mSelectedIndex = -1;
		}
	}

	mEntities = entities;
}
}
