#pragma once

#include <vector>
#include <algorithm>
#include <format>
#include <string>
#include <string_view>

#include "../../CombatTrackerLib/Utilities/UIUtils.h"
#include "../../CombatTrackerLib/Scene.h"
#include "../Entities/Creature/CreatureEnums.h"
#include "../Entities/Creature/Creature.h"

#include "imgui_stdlib.h"

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

		/*
		Constant UI labels. These should be static as they shouldn't be instantiated per object/scene.
		They are string_views, so they cost nothing to hold and they are usable at compile time.
		The concatenation these are used in goes through std::format, since a string_view has no operator+ to chain with.
		*/
		inline static constexpr std::string_view PROFICIENCY_SUFFIX{ " Proficiency" };
		inline static constexpr std::string_view EXPERTISE_SUFFIX{ " Expertise" };
		inline static constexpr std::string_view TRAIT_LABEL{ "Trait" };
		inline static constexpr std::string_view EQUIPMENT_LABEL{ "Gear" };
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
		virtual std::string_view getEntityLabel() const = 0;

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

		void renderGeneralCreatureInfo();
		void renderSpeedInfo();

		// Proficiency and expertise items get changed in this function so pass them as non-const lvalue reference.
		template <typename M, typename N>
		void renderProficiencyAndExpertiseCluster(
			std::string_view proficiencyLabel,
			std::string_view expertiseLabel,
			M& proficiencyItems,
			M& expertiseItems,
			const N& itemNames
		);
		void renderAbilityScoresAndModifiers();
		void renderSavingThrowModifiers();
		void renderSkillModifiers();
		void renderDmgVulnResAndImm();
		void renderSenses();

		// Simple helper method for rendering multi-selectable parameters.
		template <typename M, typename N>
		void renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, std::string_view label, int columnCount);

		template <typename I>
		void renderAddables(std::vector<I>& currentItems, std::string_view label);
		void renderActions();
		void renderNonOverridableInputInt(int& value, std::string_view label);

		// Pass the value as non-const lvalue reference as we need to modify its value in this function.
		template <typename T, typename M>
		void renderOverriddableInputInt (
			OverridableInt& value,
			std::string_view label,
			std::string_view overrideLabel,
			const M& items,
			const int lowerBound,
			const int upperBound,
			const T key
		);

		// Call this after creating/completing editing of Monsters/Player Characters.
		// The derived scenes extend it with the parameters that only they own.
		virtual void resetParametersAndOverrides();
	};

	/*
	A slightly more complicated helper used for user input of monster XP and PB.
	The common thing about these two types is that they set mapped values.
	Various other overridable user input can also be put in this format.
	But mapping these types is meaningless as their mapping is quite simple.
	Example: Initiative Bonus: Dexterity Modifier, Passive Perception: Perception Modifier + 10
	*/
	template <typename T, typename M>
	void EntityCreationScene::renderOverriddableInputInt(
		OverridableInt& value,
		std::string_view label,
		std::string_view overrideLabel,
		const M& items,
		const int lowerBound,
		const int upperBound,
		const T key
	) {
		if (!value.isOverridden) {
			value.value = items.at(key);
		}

		ImGui::TextUnformatted(label.data());
		ImGui::SetNextItemWidth(-1.0f);
		if (ImGui::InputInt(std::format("##{}", label).c_str(), &value.value)) {
			value.value = std::min(std::max(lowerBound, value.value), upperBound);
			value.isOverridden = true;
		}
		if (ImGui::Button(overrideLabel.data(), ImVec2{-1.0f, 0.0f})) {
			value.isOverridden = false;
		}
	}
	template <typename M, typename N>
	void EntityCreationScene::renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, std::string_view label, int columnCount) {
		ImGui::TextWrapped("%s", label.data());
		if (ImGui::BeginTable(label.data(), columnCount, ImGuiTableFlags_SizingStretchSame)) {

			// The whole table is drawn and the flags sit in an array of the same length, so one index walks both plain arrays.
			for (size_t i{}; i < names.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(names[i].data(), &items[i]);
			}
			ImGui::EndTable();
		}
	}

	/*
	Used for rendering skill and saving throw proficiencies and expertises for both monsters and player characters.
	The two clusters have a simple deduping logic between them.
	We can't have proficiency and expertise of the same "thing" at the same time.
	*/
	template <typename M, typename N>
	void EntityCreationScene::renderProficiencyAndExpertiseCluster(
		std::string_view proficiencyLabel,
		std::string_view expertiseLabel,
		M& proficiencyItems,
		M& expertiseItems,
		const N& itemNames
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
			for (size_t i{}; i < itemNames.size(); ++i) {
				if (ImGui::Checkbox(std::format("{}##{}", itemNames[i], PROFICIENCY_SUFFIX).c_str(), &proficiencyItems[i])) {
					if (proficiencyItems[i] && expertiseItems[i]) {
						expertiseItems[i] = false;
					}
				}
			}

			// Expertise checkboxes.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel.data());
			for (size_t i{}; i < itemNames.size(); ++i) {
				if (ImGui::Checkbox(std::format("{}##{}", itemNames[i], EXPERTISE_SUFFIX).c_str(), &expertiseItems[i])) {
					if (expertiseItems[i] && proficiencyItems[i]) {
						proficiencyItems[i] = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Chunky helper used for rendering traits and gear items.
	template <typename I>
	void EntityCreationScene::renderAddables(std::vector<I>& currentItems, std::string_view label) {
		ImGui::TextWrapped("%s", label.data());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> s_markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {

			// Cache the string to prevent recomputing it.
			const std::string popupName{ std::format("Edit {}##{}", label, i) };

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
				ImGui::OpenPopup(popupName.c_str());
			}

			// Setup configs of the edit popup of items.
			// The popup can neither be moved nor resized by hand, so it simply keeps following the viewport.
			const auto center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
			const auto vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);

			// Popup modal for editing/deleting items.
			if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

				// The layouting is quite similar to the big entity creation popup modal.
				ImGui::BeginChild("EditAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(2) }, true);
				ImGui::InputText(std::format("{} Name", label).c_str(), &currentItems[i].name);
				ImGui::InputText(std::format("{} Description", label).c_str(), &currentItems[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(std::format("Finish {} Editing", label).c_str(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(std::format("Delete {}", label).c_str(), 0.5f)) {
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
			currentItems.erase(currentItems.begin() + s_markedForDeletion[i]);
		}
		s_markedForDeletion.clear();

		// Add item section.
		ImGui::Spacing();
		if (ImGui::Button(std::format("Add {}", label).c_str())) {
			ImGui::OpenPopup(label.data());
		}

		// Setup configs of the add item popup.
		const auto center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
		const auto vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Always);
		if (ImGui::BeginPopupModal(label.data(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("AddAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true);

			// Again it is fine for these to be static, we only use them here and reset them after adding the item.
			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText(std::format("{} Name", label).c_str(), &s_name);
			ImGui::InputText(std::format("{} Description", label).c_str(), &s_description);
			ImGui::EndChild();

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(std::format("Finish {} Creation", label).c_str(), 0.5f) &&
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
