#pragma once

#include "CombatTrackerApp/Entities/Creature.h"
#include "CombatTrackerLib/ImGuiScopes.h"
#include "CombatTrackerLib/Scene.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"

#include <algorithm>
#include <cstddef>
#include <imgui_stdlib.h>
#include <span>
#include <string>
#include <string_view>
#include <vector>

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

		struct ModifierTableLabels {
			std::string_view header{};
			std::string_view itemColumn{};
			std::string_view valueColumn{};
			std::string_view resetColumn{};
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
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
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

		// A specific section below the senses. Only player characters currently fill it so it defaults to rendering nothing
		virtual void renderEntitySpecificSection() {}

		virtual bool completeEntityCreation() = 0;

		// Resolving needs the entity specific block alongside the creature one, which only the derived
		// scene owns, so the base scene provides a virtual function for this purpose.
		virtual void resolveDerivedValues() = 0;

		// The block is edited in place by this base scene and by the derived ones alike.
		CreatureStatBlock m_currentCreatureStatBlock{};

		CombatTrackerLib::ListBoxParameters m_sizeListBoxParameters{};
		CombatTrackerLib::ListBoxParameters m_creatureTypeParameters{};
		CombatTrackerLib::ListBoxParameters m_alignmentParameters{};

		// Provide a span of proficiency levels as our array size can differ (skills vs saving throws).
		static void renderProficiencyAndExpertiseCluster(
			std::string_view proficiencyLabel,
			std::string_view expertiseLabel,
			std::span<ProficiencyLevel> proficiencyLevels,
			std::span<const std::string_view> itemNames
		);

		// A plain checkbox grid over proficiency levels, for the items that only ever toggle between
		// having a proficiency and not having one.
		static void renderProficiencyLevelGrid(
			std::span<ProficiencyLevel> proficiencyLevels,
			std::span<const std::string_view> itemNames,
			std::string_view stableID,
			std::string_view label,
			int columnCount
		);

		/*
		Renders a list of items where each one is a button opening an edit popup, plus an add popup.
		The extended form takes the three things an item type can differ in:
		what extra widgets the add popup shows, how a finished item is built, and what the tooltip says.
		The simple form covers anything that is just a name and a description.
		*/
		template <typename I, typename AddFieldsFn, typename MakeItemFn, typename TooltipFn>
		static void renderAddables(
			std::vector<I>& currentItems,
			const AddableLabels& labels,
			AddFieldsFn&& renderAddFields,
			MakeItemFn&& makeItem,
			TooltipFn&& renderTooltip
		);
		template <typename I> static void renderAddables(std::vector<I>& currentItems, const AddableLabels& labels);

		/*
		Saving throws and skills present the same table, a name beside an overridable value beside a
		reset button, so both go through here and differ only in what they hand over.
		The column widths are passed in as the two tables sit in differently proportioned columns.
		*/
		static void renderModifierTable(
			std::string_view stableID,
			const ModifierTableLabels& labels,
			std::span<CombatTrackerLib::OverridableInt> modifiers,
			std::span<const std::string_view> itemNames,
			std::span<const std::string_view> resetLabels,
			std::span<const float, 3> columnWidths
		);

		// The general info is four unrelated tables stacked together, so each one gets its own function
		// and renderGeneralCreatureInfo() is left as the layout that arranges them.
		void renderGeneralCreatureInfo();
		void renderNameRow();
		void renderCreatureListBoxes();
		void renderDerivedOverridables();
		void renderArmorClassAndHitPoints();

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

	// Chunky helper used for rendering traits, gear and actions alike.
	template <typename I, typename AddFieldsFn, typename MakeItemFn, typename TooltipFn>
	void EntityCreationScene::renderAddables(
		std::vector<I>& currentItems,
		const AddableLabels& labels,
		AddFieldsFn&& renderAddFields,
		MakeItemFn&& makeItem,
		TooltipFn&& renderTooltip
	) {
		ImGui::TextWrapped("%s", labels.header.data());

		// It is fine for this to be actually static as we clear it on each render pass anyways.
		// Furthermore, this vector is only used in this function and nowhere else.
		static std::vector<size_t> s_markedForDeletion{};
		for (size_t i{}; i < currentItems.size(); ++i) {

			// Push a separate id to the ID stack for each item.
			CombatTrackerLib::ScopedID scopedID{ static_cast<int>(i) };

			/*
			We should check separate IO events explicitly instead of generically checking for any IO events for these buttons.
			Unlike most of the widgets. This is because we need a tooltip for each button.
			The items are listed one per row, so the button takes the full width and no SameLine() follows it.
			*/
			ImGui::Button(currentItems[i].name.c_str(), ImVec2{ -1.0f, 0.0f });
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				renderTooltip(currentItems[i]);
			}
			if (ImGui::IsItemClicked()) {
				ImGui::OpenPopup(labels.edit.data());
			}

			// Setup configs of the edit popup of items.
			// The popup can neither be moved nor resized by hand, so it simply keeps following the viewport.
			CombatTrackerLib::centerNextWindow(0.70f, 0.70f);

			// Popup modal for editing/deleting items.
			if (CombatTrackerLib::ScopedPopupModal popupModal{
					labels.edit, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize }) {

				// The layouting is quite similar to the big entity creation popup modal.
				{
					CombatTrackerLib::ScopedChild editAddableScroll{
						"EditAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(2) }, true
					};
					ImGui::InputText(labels.name.data(), &currentItems[i].name);
					ImGui::InputText(labels.description.data(), &currentItems[i].description);
				}

				ImGui::Separator();
				if (CombatTrackerLib::alignedButton(labels.finishEditing.data(), 0.5f)) {
					ImGui::CloseCurrentPopup();
				}
				if (CombatTrackerLib::alignedButton(labels.remove.data(), 0.5f)) {
					s_markedForDeletion.push_back(i);
					ImGui::CloseCurrentPopup();
				}
			}
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
		CombatTrackerLib::centerNextWindow(0.70f, 0.70f);
		if (CombatTrackerLib::ScopedPopupModal popupModal{
				labels.add, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize }) {

			/*
			These are static so they survive between frames while the popup is open.
			They are cleared as the popup appears rather than only on a successful add, as traits and
			equipment share this instantiation and would otherwise hand each other their leftover text.
			The check sits out here rather than inside the scrolling child, so it asks whether the popup
			appeared rather than whether the child did.
			*/
			static std::string s_name{};
			static std::string s_description{};
			if (ImGui::IsWindowAppearing()) {
				s_name.clear();
				s_description.clear();
			}
			{
				CombatTrackerLib::ScopedChild addAddableScroll{
					"AddAddableScroll", ImVec2{ 0.0f, -CombatTrackerLib::reservedFooterHeight(1) }, true
				};
				renderAddFields();
				ImGui::InputText(labels.name.data(), &s_name);
				ImGui::InputText(labels.description.data(), &s_description);
			}

			// Button and cleanup section.
			ImGui::Separator();
			if (CombatTrackerLib::alignedButton(labels.finishCreation.data(), 0.5f) && !s_name.empty() &&
				!s_description.empty()) {
				currentItems.push_back(makeItem(s_name, s_description));
				s_name.clear();
				s_description.clear();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	// Anything that is just a name and a description needs none of the hooks.
	template <typename I>
	void EntityCreationScene::renderAddables(std::vector<I>& currentItems, const AddableLabels& labels) {
		renderAddables(
			currentItems,
			labels,
			[] {},
			[](const std::string& name, const std::string& description) { return I{ name, description }; },
			[](const I& item) { ImGui::SetTooltip("%s", item.description.c_str()); }
		);
	}
}
