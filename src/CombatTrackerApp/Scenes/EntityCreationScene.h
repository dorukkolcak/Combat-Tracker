#pragma once

#include "CombatTrackerLib/Utilities/UIUtils.h"
#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerApp/Entities/Creature/CreatureEnums.h"
#include "CombatTrackerApp/Entities/Creature/Creature.h"

#include <vector>
#include <algorithm>
#include <span>
#include <string>
#include <string_view>
#include <cstddef>
#include <imgui_stdlib.h>

namespace CombatTrackerApp {

	// The half of entity creation that monsters and player characters share.
	// Whatever differs between the two entities is left to the derived scenes as pure virtuals.
	class EntityCreationScene : public CombatTrackerLib::Scene {
	public:

		explicit EntityCreationScene(CombatTrackerLib::App* app);

		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;

	protected:

		struct CreationLabels {
			std::string_view windowTitle{};
			std::string_view completeButton{};
			std::string_view cancelButton{};
		};

		struct AddableLabels {
			std::string_view header{};
			std::string_view add{};
			std::string_view edit{};
			std::string_view name{};
			std::string_view description{};
			std::string_view finishCreation{};
			std::string_view finishEditing{};
			std::string_view remove{};
		};

		inline static constexpr float MIN_CHILD_HEIGHT{ 180.f };
		inline static constexpr ImGuiWindowFlags ENTITY_CREATION_WINDOW_FLAGS{
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking
		};

		/*
		I kinda hate writing functions like this where it is a meaningless wrapper over a single return statement.
		But in this case we actually need the polymorphic behaviour (both of the derived classes return a different value here.)
		So I guess it is fine.
		*/
		virtual CreationLabels getCreationLabels() const = 0;

		// Right hand column of the top split (of the popup). Monster specific data for monsters, classes and subclasses for player characters.
		virtual void renderEntitySpecificInfo() = 0;

		// A specific section below the senses. Only player characters currently fill it.
		// Monster provides an empty override in this case.
		virtual void renderEntitySpecificSection() = 0;

		virtual bool completeEntityCreation() = 0; 

		/*
		The block is edited in place by this base scene and by the derived ones alike.
		The derived scene moves it into the entity it builds once creation is completed,
		which is why resetParametersAndOverrides() puts a fresh block back in its place.
		*/
		CreatureStatBlock m_currentCreatureStatBlock{};

		CombatTrackerLib::ListBoxParameters m_sizeListBoxParameters{};
		CombatTrackerLib::ListBoxParameters m_creatureTypeParameters{};
		CombatTrackerLib::ListBoxParameters m_alignmentParameters{};

		// Proficiency and expertise items get changed in this function so pass them as non-const lvalue reference.
		template <typename M>
		static void renderProficiencyAndExpertiseCluster(
			std::string_view proficiencyLabel,
			std::string_view expertiseLabel,
			M& proficiencyItems,
			M& expertiseItems,
			std::span<const std::string_view> itemNames
		);
		template <typename I>
		static void renderAddables(std::vector<I>& currentItems, const AddableLabels& labels);

		void renderGeneralCreatureInfo();
		void renderSpeedInfo();
		void renderAbilityScoresAndModifiers();
		void renderSavingThrowModifiers();
		void renderSkillModifiers();
		void renderDmgVulnResAndImm();
		void renderSenses();
		void renderActions();

		// Call this after creating/completing editing of Monsters/Player Characters.
		// The derived scenes extend it with the parameters that only they own.
		virtual void resetParametersAndOverrides();
	};

	/*
	Used for rendering skill and saving throw proficiencies and expertises for both monsters and player characters.
	The two clusters have a simple deduping logic between them.
	We can't have proficiency and expertise of the same "thing" at the same time.
	*/
	template <typename M>
	void EntityCreationScene::renderProficiencyAndExpertiseCluster(
		std::string_view proficiencyLabel,
		std::string_view expertiseLabel,
		M& proficiencyItems,
		M& expertiseItems,
		std::span<const std::string_view> itemNames
	) {
		if (ImGui::BeginTable("ProfExpTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {

			ImGui::TableSetupColumn("Left");
			ImGui::TableSetupColumn("Right");
			ImGui::TableNextRow();

			// Proficiency checkboxes.
			// The names and both flag arrays are the same length, so a single index walks all three plain arrays.
			// This is also why the helper works for abilities and skills alike, as nothing here is tied to one enum.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", proficiencyLabel.data());
			ImGui::PushID("Proficiency");
			for (size_t i{}; i < itemNames.size(); ++i) {
				if (ImGui::Checkbox(itemNames[i].data(), &proficiencyItems[i])) {
					if (proficiencyItems[i] && expertiseItems[i]) {
						expertiseItems[i] = false;
					}
				}
			}
			ImGui::PopID();

			// Expertise checkboxes.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel.data());
			ImGui::PushID("Expertise");
			for (size_t i{}; i < itemNames.size(); ++i) {
				if (ImGui::Checkbox(itemNames[i].data(), &expertiseItems[i])) {
					if (expertiseItems[i] && proficiencyItems[i]) {
						proficiencyItems[i] = false;
					}
				}
			}
			ImGui::PopID();
			ImGui::EndTable();
		}
	}

	// Chunky helper used for rendering traits and gear items.
	template <typename I>
	void EntityCreationScene::renderAddables(std::vector<I>& currentItems, const AddableLabels& labels) {
		ImGui::TextWrapped("%s", labels.header.data());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> s_markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {

			// Push a separate id to the ID stack for each item.
			ImGui::PushID(static_cast<int>(i));

			/*
			We should check separate IO events explicitly instead of generically checking for any IO events for these buttons.
			Unlike most of the widgets. This is because we need a tooltip for each button.
			The items are listed one per row, so the button takes the full width and no SameLine() follows it.
			*/
			ImGui::Button(currentItems[i].name.c_str(), ImVec2{ -1.0f, 0.0f });
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s", currentItems[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(labels.edit.data());
			}

			// Setup configs of the edit popup of items.
			// The popup can neither be moved nor resized by hand, so it simply keeps following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);

			// Popup modal for editing/deleting items.
			if (ImGui::BeginPopupModal(labels.edit.data(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

				// The layouting is quite similar to the big entity creation popup modal.
				ImGui::BeginChild("EditAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(2) }, true);
				ImGui::InputText(labels.name.data(), &currentItems[i].name);
				ImGui::InputText(labels.description.data(), &currentItems[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(labels.finishEditing.data(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(labels.remove.data(), 0.5f)) {
					s_markedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		// Delete marked items safely, starting from the largest index to the smallest index.
		std::sort(s_markedForDeletion.begin(), s_markedForDeletion.end());
		for (size_t i = s_markedForDeletion.size(); i-- > 0;) {
			currentItems.erase(currentItems.begin() + static_cast<std::ptrdiff_t>(s_markedForDeletion[i]));
		}
		s_markedForDeletion.clear();

		// Add item section.
		ImGui::Spacing();
		if (ImGui::Button(labels.add.data())) {
			ImGui::OpenPopup(labels.add.data());
		}

		// Setup configs of the add item popup.
		const auto center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
		const auto vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);
		if (ImGui::BeginPopupModal(labels.add.data(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("AddAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);

			// Again it is fine for these to be static, we only use them here and reset them after adding the item.
			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText(labels.name.data(), &s_name);
			ImGui::InputText(labels.description.data(), &s_description);
			ImGui::EndChild();

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(labels.finishCreation.data(), 0.5f) &&
				!s_name.empty() && !s_description.empty()) {
				I item(s_name, s_description);
				currentItems.push_back(item);
				s_name.clear();
				s_description.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}
