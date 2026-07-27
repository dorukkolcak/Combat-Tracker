#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

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
		The reason the type is string not string_view for most of the UI labels in this class is that I frequently do concatenation with them.
		So I need an underlying string type to work with, which is annoying to access with a string_view.
		Probably can make it work at some point and be more efficient but this is fine for now.
		*/
		inline static const std::string PROFICIENCY_SUFFIX{ " Proficiency" };
		inline static const std::string EXPERTISE_SUFFIX{ " Expertise" };
		inline static const std::string TRAIT_LABEL{ "Trait" };
		inline static const std::string EQUIPMENT_LABEL{ "Gear" };
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

		// What the entity is called. Used for the header and for every footer button label.
		virtual const std::string& getEntityLabel() const = 0;

		// Legendary and lair actions are monster only, so the popup has to know which entity it serves.
		virtual bool isMonsterScene() const = 0;

		// Right hand column of the top split. Monster specific data for monsters, classes and subclasses for player characters.
		virtual void renderEntitySpecificInfo() = 0;

		// A specific section below the senses. Only player characters currently fill it.
		virtual void renderEntitySpecificSection() = 0;

		// Returning true hands the scene back to the encounter creation scene.
		// Returning false keeps this scene up, so that a derived scene can refuse an incomplete entity.
		virtual bool completeEntityCreation() = 0;

		// Called on the first render pass so that the scene always starts on a fresh entity.
		virtual void resetEntity() = 0;

		/*
		The block is owned by value here instead of being a reference into a builder.
		The builders belong to the derived scenes, which are constructed after this base runs,
		so a reference member would have to bind to a member that does not exist yet.
		The derived scene moves this block into its own builder once creation is completed.
		*/
		CreatureStatBlock m_currentCreatureStatBlock{};

		CombatTrackerLib::ListBoxParameters m_sizeListBoxParameters{};
		CombatTrackerLib::ListBoxParameters m_creatureTypeParameters{};
		CombatTrackerLib::ListBoxParameters m_alignmentParameters{};

		bool m_isInitiativeOverridden{};
		bool m_isPassivePerceptionOverridden{};

		std::unordered_map<Ability, bool> m_abilityModifiersOverride{};
		std::unordered_map<Ability, bool> m_savingThrowsOverride{};
		std::unordered_map<Skill, bool> m_skillModifiersOverride{};

		void renderGeneralCreatureInfo();
		void renderSpeedInfo();

		// Proficiency and expertise items get changed in this function so pass them as non-const lvalue reference.
		template <typename M, typename N>
		void renderProficiencyAndExpertiseCluster(
			const char* proficiencyLabel,
			const char* expertiseLabel,
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
		void renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, const char* label, int columnCount);

		template <typename I>
		void renderAddables(std::vector<I>& currentItems, const std::string& label);
		void renderActions(bool isMonster);
		void renderNonOverridableInputInt(int& value, const char* label);

		// Pass the value as non-const lvalue reference as we need to modify its value in this function.
		template <typename T, typename M>
		void renderOverriddableInputInt (
			int& value,
			bool& overrideFlag,
			const char* label,
			const char* overrideLabel,
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
		int& value,
		bool& overrideFlag,
		const char* label,
		const char* overrideLabel,
		const M& items,
		const int lowerBound,
		const int upperBound,
		const T key
	) {
		if (!overrideFlag) {
			value = items.at(key);
		}
		if (ImGui::InputInt(label, &value)) {
			value = std::min(std::max(lowerBound, value), upperBound);
			overrideFlag = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(overrideLabel)) {
			overrideFlag = false;
		}
	}
	template <typename M, typename N>
	void EntityCreationScene::renderMutuallyExclusiveCheckboxCluster(M& items, const N& names, const char* label, int columnCount) {
		ImGui::TextWrapped("%s", label);
		if (ImGui::BeginTable(label, columnCount, ImGuiTableFlags_SizingStretchSame)) {
			int counter{};
			for (const auto& item : names) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(item.second.c_str(), &items.at(item.first));
				++counter;
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
		const char* proficiencyLabel,
		const char* expertiseLabel,
		M& proficiencyItems,
		M& expertiseItems,
		const N& itemNames
	) {
		if (ImGui::BeginTable("ProfExpTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV)) {

			ImGui::TableSetupColumn("Left");
			ImGui::TableSetupColumn("Right");
			ImGui::TableNextRow();

			// Proficiency checkboxes.
			ImGui::TableSetColumnIndex(0);
			ImGui::TextWrapped("%s", proficiencyLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + PROFICIENCY_SUFFIX).c_str(), &proficiencyItems.at(item.first))) {
					if (proficiencyItems.at(item.first) && expertiseItems.at(item.first)) {
						expertiseItems.at(item.first) = false;
					}
				}
			}

			// Expertise checkboxes.
			ImGui::TableSetColumnIndex(1);
			ImGui::TextWrapped("%s", expertiseLabel);
			for (const auto& item : itemNames) {
				if (ImGui::Checkbox((item.second + "##" + EXPERTISE_SUFFIX).c_str(), &expertiseItems.at(item.first))) {
					if (expertiseItems.at(item.first) && proficiencyItems.at(item.first)) {
						proficiencyItems.at(item.first) = false;
					}
				}
			}
			ImGui::EndTable();
		}
	}

	// Chunky helper used for rendering traits and gear items.
	template <typename I>
	void EntityCreationScene::renderAddables(std::vector<I>& currentItems, const std::string& label) {
		ImGui::TextWrapped("%s", label.c_str());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> s_markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {
			ImGui::TableNextColumn();

			// Cache the string to prevent recomputing it.
			std::string popupName{ ("Edit " + label + "##" + std::to_string(i)).c_str() };

			// Push a separate id to the ID stack for each item.
			ImGui::PushID(static_cast<int>(i));

			// We should check separate IO events explicitly instead of generically checking for any IO events for these buttons.
			// Unlike most of the widgets. This is because we need a tooltip for each button.
			ImGui::Button(currentItems[i].name.c_str());
			ImGui::SameLine();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("%s", currentItems[i].description.c_str());
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(popupName.c_str());
			}

			// Setup configs of the edit popup of items.
			const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
			const ImVec2 vp{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);

			// Popup modal for editing/deleting items.
			if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

				// The layouting is quite similar to the big entity creation popup modal.
				ImGui::BeginChild("EditAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);
				ImGui::InputText((label + " Name").c_str(), &currentItems[i].name);
				ImGui::InputText((label + " Description").c_str(), &currentItems[i].description);
				ImGui::EndChild();

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(("Finish " + label + " Editing").c_str(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(("Delete " + label).c_str(), 0.5f)) {
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
		if (ImGui::Button(("Add " + label).c_str())) {
			ImGui::OpenPopup(label.c_str());
		}

		// Setup configs of the add item popup.
		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{ 0.5f, 0.5f });
		ImVec2 vp{ ImGui::GetMainViewport()->Size };
		ImGui::SetNextWindowSize(ImVec2{ vp.x * 0.70f, vp.y * 0.70f }, ImGuiCond_Appearing);
		if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
			ImGui::BeginChild("AddAddableScroll", ImVec2{ 0, -ImGui::GetFrameHeightWithSpacing() * 2.0f }, true);

			// Again it is fine for these to be static, we only use them here and reset them after adding the item.
			static std::string s_name{};
			static std::string s_description{};
			ImGui::InputText((label + " Name").c_str(), &s_name);
			ImGui::InputText((label + " Description").c_str(), &s_description);
			ImGui::EndChild();

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(("Finish " + label + " Creation").c_str(), 0.5f) &&
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
