#pragma once

#include <imgui.h>

#include <string_view>

namespace CombatTrackerLib {

	// This file provides a wrapper over ImGui widgets using RAII in an attempt to reduce the boilerplate code of ImGui widget calls.
	// Writing Begin()/End() hundreds of times tends to get annoying unfortunately.

	// Generic base class for any RAII UI widget.
	class ImGuiScope {
	public:
		ImGuiScope(const ImGuiScope&) = delete;
		ImGuiScope& operator=(const ImGuiScope&) = delete;
		ImGuiScope(ImGuiScope&&) = delete;
		ImGuiScope& operator=(ImGuiScope&&) = delete;

	protected:
		ImGuiScope() = default;
		~ImGuiScope() = default;
	};

	/*
	The base class for widgets that reports whether its contents are worth submitting, so it can be used in an if statement.
	The conversion is explicit, which is all an if needs, while still refusing to silently become an int.
	The stack scopes below deliberately do not derive from this, as there is nothing to test and an if
	around one of them would be a mistake rather.
	*/
	class ImGuiConditionalScope : public ImGuiScope {
	public:
		explicit operator bool() const;

	protected:
		explicit ImGuiConditionalScope(bool isOpen);
		~ImGuiConditionalScope() = default;

		bool m_isOpen{};
	};

	// The widgets below close only when they opened.
	// Their contents belong inside an if, as ImGui skips them entirely when the scope is not open.
	class ScopedTable : public ImGuiConditionalScope {
	public:
		ScopedTable(
			std::string_view stableID,
			int columnCount,
			ImGuiTableFlags flags = 0,
			const ImVec2& outerSize = ImVec2{ 0.0f, 0.0f },
			float innerWidth = 0.0f
		);
		~ScopedTable();
	};
	class ScopedListBox : public ImGuiConditionalScope {
	public:
		ScopedListBox(std::string_view label, const ImVec2& size = ImVec2{ 0.0f, 0.0f });
		~ScopedListBox();
	};

	// Note that a popup modal closes with EndPopup rather than a matching EndPopupModal.
	class ScopedPopupModal : public ImGuiConditionalScope {
	public:
		ScopedPopupModal(std::string_view name, bool* isOpenPointer = nullptr, ImGuiWindowFlags flags = 0);
		~ScopedPopupModal();
	};
	class ScopedTabBar : public ImGuiConditionalScope {
	public:
		ScopedTabBar(std::string_view stableID, ImGuiTabBarFlags flags = 0);
		~ScopedTabBar();
	};
	class ScopedTabItem : public ImGuiConditionalScope {
	public:
		ScopedTabItem(std::string_view label, bool* isOpenPointer = nullptr, ImGuiTabItemFlags flags = 0);
		~ScopedTabItem();
	};
	class ScopedMenuBar : public ImGuiConditionalScope {
	public:
		ScopedMenuBar();
		~ScopedMenuBar();
	};
	class ScopedMenu : public ImGuiConditionalScope {
	public:
		ScopedMenu(std::string_view label, bool isEnabled = true);
		~ScopedMenu();
	};

	// These two widgets close regardless of their open/close status.
	// So their closing call must not sit behind the same if.
	class ScopedWindow : public ImGuiConditionalScope {
	public:
		ScopedWindow(std::string_view name, bool* isOpenPointer = nullptr, ImGuiWindowFlags flags = 0);
		~ScopedWindow();
	};
	class ScopedChild : public ImGuiConditionalScope {
	public:
		ScopedChild(
			std::string_view stableID,
			const ImVec2& size = ImVec2{ 0.0f, 0.0f },
			ImGuiChildFlags childFlags = 0,
			ImGuiWindowFlags windowFlags = 0
		);
		~ScopedChild();
	};

	// The stack scopes below have nothing to report, they simply undo themselves.
	// They are separate types rather than one, as each pops from a different ImGui stack.
	class ScopedID : public ImGuiScope {
	public:
		explicit ScopedID(std::string_view stableID);
		explicit ScopedID(int stableID);
		~ScopedID();
	};
	class ScopedStyleVar : public ImGuiScope {
	public:
		ScopedStyleVar(ImGuiStyleVar styleVariable, float value);
		ScopedStyleVar(ImGuiStyleVar styleVariable, const ImVec2& value);
		~ScopedStyleVar();
	};
	class ScopedStyleColor : public ImGuiScope {
	public:
		ScopedStyleColor(ImGuiCol styleColor, const ImVec4& value);
		~ScopedStyleColor();
	};

	// The unscaled size is what PushFont takes, where zero keeps whatever size is already current.
	class ScopedFont : public ImGuiScope {
	public:
		ScopedFont(ImFont* font, float unscaledSize = 0.0f);
		~ScopedFont();
	};
	class ScopedDisabled : public ImGuiScope {
	public:
		explicit ScopedDisabled(bool isDisabled = true);
		~ScopedDisabled();
	};
}
