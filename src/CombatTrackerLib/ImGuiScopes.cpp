#include "CombatTrackerLib/ImGuiScopes.h"

namespace CombatTrackerLib {

	// The default arguments live on the declarations in the header, so they are not repeated here.
	ImGuiConditionalScope::ImGuiConditionalScope(bool isOpen) : m_isOpen{isOpen} {}

	ImGuiConditionalScope::operator bool() const {
		return m_isOpen;
	}

	ScopedTable::ScopedTable(
		std::string_view stableID,
		int columnCount,
		ImGuiTableFlags flags,
		const ImVec2& outerSize,
		float innerWidth
	)
		: ImGuiConditionalScope{ImGui::BeginTable(stableID.data(), columnCount, flags, outerSize, innerWidth)} {}

	ScopedTable::~ScopedTable() {
		if (m_isOpen) {
			ImGui::EndTable();
		}
	}

	ScopedListBox::ScopedListBox(std::string_view label, const ImVec2& size)
		: ImGuiConditionalScope{ImGui::BeginListBox(label.data(), size)} {}

	ScopedListBox::~ScopedListBox() {
		if (m_isOpen) {
			ImGui::EndListBox();
		}
	}

	ScopedPopupModal::ScopedPopupModal(std::string_view name, bool* isOpenPointer, ImGuiWindowFlags flags)
		: ImGuiConditionalScope{ImGui::BeginPopupModal(name.data(), isOpenPointer, flags)} {}

	ScopedPopupModal::~ScopedPopupModal() {
		if (m_isOpen) {
			ImGui::EndPopup();
		}
	}

	ScopedTabBar::ScopedTabBar(std::string_view stableID, ImGuiTabBarFlags flags)
		: ImGuiConditionalScope{ImGui::BeginTabBar(stableID.data(), flags)} {}

	ScopedTabBar::~ScopedTabBar() {
		if (m_isOpen) {
			ImGui::EndTabBar();
		}
	}

	ScopedTabItem::ScopedTabItem(std::string_view label, bool* isOpenPointer, ImGuiTabItemFlags flags)
		: ImGuiConditionalScope{ImGui::BeginTabItem(label.data(), isOpenPointer, flags)} {}

	ScopedTabItem::~ScopedTabItem() {
		if (m_isOpen) {
			ImGui::EndTabItem();
		}
	}

	ScopedMenuBar::ScopedMenuBar() : ImGuiConditionalScope{ImGui::BeginMenuBar()} {}

	ScopedMenuBar::~ScopedMenuBar() {
		if (m_isOpen) {
			ImGui::EndMenuBar();
		}
	}

	ScopedMenu::ScopedMenu(std::string_view label, bool isEnabled)
		: ImGuiConditionalScope{ImGui::BeginMenu(label.data(), isEnabled)} {}

	ScopedMenu::~ScopedMenu() {
		if (m_isOpen) {
			ImGui::EndMenu();
		}
	}

	ScopedWindow::ScopedWindow(std::string_view name, bool* isOpenPointer, ImGuiWindowFlags flags)
		: ImGuiConditionalScope{ImGui::Begin(name.data(), isOpenPointer, flags)} {}

	ScopedWindow::~ScopedWindow() {
		ImGui::End();
	}

	ScopedChild::ScopedChild(
		std::string_view stableID,
		const ImVec2& size,
		ImGuiChildFlags childFlags,
		ImGuiWindowFlags windowFlags
	)
		: ImGuiConditionalScope{ImGui::BeginChild(stableID.data(), size, childFlags, windowFlags)} {}

	ScopedChild::~ScopedChild() {
		ImGui::EndChild();
	}

	ScopedID::ScopedID(std::string_view stableID) {
		ImGui::PushID(stableID.data());
	}

	ScopedID::ScopedID(int stableID) {
		ImGui::PushID(stableID);
	}

	ScopedID::~ScopedID() {
		ImGui::PopID();
	}

	ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVariable, float value) {
		ImGui::PushStyleVar(styleVariable, value);
	}

	ScopedStyleVar::ScopedStyleVar(ImGuiStyleVar styleVariable, const ImVec2& value) {
		ImGui::PushStyleVar(styleVariable, value);
	}

	ScopedStyleVar::~ScopedStyleVar() {
		ImGui::PopStyleVar();
	}

	ScopedStyleColor::ScopedStyleColor(ImGuiCol styleColor, const ImVec4& value) {
		ImGui::PushStyleColor(styleColor, value);
	}

	ScopedStyleColor::~ScopedStyleColor() {
		ImGui::PopStyleColor();
	}

	ScopedFont::ScopedFont(ImFont* font, float unscaledSize) {
		ImGui::PushFont(font, unscaledSize);
	}

	ScopedFont::~ScopedFont() {
		ImGui::PopFont();
	}

	ScopedDisabled::ScopedDisabled(bool isDisabled) {
		ImGui::BeginDisabled(isDisabled);
	}

	ScopedDisabled::~ScopedDisabled() {
		ImGui::EndDisabled();
	}
}
