#include "CombatTrackerApp/Scenes/MainMenuScene.h"
#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"
#include "CombatTrackerLib/Utilities/UIUtils.h"

#include <glad/glad.h>
#include <algorithm>
#include <imgui.h>
#include <optional>
#include <string>
#include <string_view>

namespace CombatTrackerApp {

	MainMenuScene::MainMenuScene(CombatTrackerLib::App* app)
		: CombatTrackerLib::Scene{ app },
		  m_titleTexture{ app->getDirectoryProvider().getResourceDirectory() /
							  "assets" / "Textures" / "Title.png",
						  4,
						  false,
						  GL_REPEAT,
						  GL_REPEAT,
						  GL_LINEAR,
						  GL_LINEAR_MIPMAP_LINEAR },
		  m_selectedOptionsCategory{}, m_isDockspaceInitialized{ false } {
		m_titleWindowClass.DockNodeFlagsOverrideSet =
			ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoSplit |
			ImGuiDockNodeFlags_NoTabBar;
	}
	void MainMenuScene::notifyScene(const ImGuiIO&) {}
	void MainMenuScene::updateScene(double) {}
	void MainMenuScene::renderUI() {

		std::optional<std::string_view> pendingLocaleCode{};

		// Set configurations of the host window of the scene.
		// This has to happen on every pass and not just the initial one, otherwise the host stays at its old size.
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		// Create the dockplace settings and layout on initial render pass.
		if (!m_isDockspaceInitialized) {

			m_dockspaceID = ImGui::GetID("Main Menu Dockspace");
			ImGui::DockBuilderRemoveNode(m_dockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(
				m_dockspaceID, ImGuiDockNodeFlags_DockSpace
			);
			ImGui::DockBuilderSetNodeSize(m_dockspaceID, viewport->WorkSize);

			// Configure dockspace layout.
			ImGuiID bottom{ m_dockspaceID };
			ImGui::DockBuilderSplitNode(
				m_dockspaceID, ImGuiDir_Up, 0.60f, &m_titleWindowID, &bottom
			);
			ImGui::DockBuilderSplitNode(
				bottom,
				ImGuiDir_Left,
				0.33f,
				&m_encounterCreationWindowID,
				&m_encounterRunningWindowID
			);
			ImGui::DockBuilderSplitNode(
				m_encounterRunningWindowID,
				ImGuiDir_Right,
				0.50f,
				&m_completedEncountersWindowID,
				&m_encounterRunningWindowID
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				m_localeProviderPointer->getText("/scenes/mainMenu/titleWindow")
					.data(),
				m_titleWindowID
			);
			ImGui::DockBuilderDockWindow(
				m_localeProviderPointer
					->getText("/scenes/mainMenu/encounterCreation/windowTitle")
					.data(),
				m_encounterCreationWindowID
			);
			ImGui::DockBuilderDockWindow(
				m_localeProviderPointer
					->getText("/scenes/mainMenu/runEncounter/windowTitle")
					.data(),
				m_encounterRunningWindowID
			);
			ImGui::DockBuilderDockWindow(
				m_localeProviderPointer
					->getText("/scenes/mainMenu/completedEncounters/windowTitle"
					)
					.data(),
				m_completedEncountersWindowID
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(m_dockspaceID);
			m_isDockspaceInitialized = true;
		}

		// Render the host window. The host window is pure scaffolding, so its padding and border are removed completely.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		{
			CombatTrackerLib::ScopedWindow hostWindow{ "Host Window",
													   nullptr,
													   HOST_WINDOW_FLAGS };
			ImGui::PopStyleVar(3);

			// Render the dockspace of the host window.
			ImGui::DockSpace(
				m_dockspaceID, ImGui::GetContentRegionAvail(), DOCKSPACE_FLAGS
			);
		}

		// Title window.
		ImGui::SetNextWindowClass(&m_titleWindowClass);
		{
			CombatTrackerLib::ScopedWindow titleWindow{
				m_localeProviderPointer->getText("/scenes/mainMenu/titleWindow")
			};

			const auto optionsLabel{
				m_localeProviderPointer->getText("/scenes/mainMenu/options")
			};
			const auto exitLabel{
				m_localeProviderPointer->getText("/scenes/mainMenu/exit")
			};

			/*
            The two buttons are aligned as one row instead of individually.
            Aligning only the first one pushes the second one off the right edge once the window gets narrow.
            I thought initially this should be a constexpr static variable as the width is set.
            But the ImGui style can change during run time through changing the UI preset so this has to be calculated every render.
            */
			const auto buttonRowWidth{
				CombatTrackerLib::buttonWidth(optionsLabel.data()) +
				CombatTrackerLib::buttonWidth(exitLabel.data()) +
				ImGui::GetStyle().ItemSpacing.x
			};
			CombatTrackerLib::alignCursorForWidth(buttonRowWidth, 1.0f);
			if (ImGui::Button(optionsLabel.data())) {
				ImGui::OpenPopup("Options Popup");
			}
			ImGui::SameLine();
			if (ImGui::Button(exitLabel.data())) {

				// This signal should be deferred and handled by the application instance at the start of the frame.
				m_applicationPointer->signalApplicationClose();
			}

			// Tinting with a style colour makes the logo follow the active UI preset.
			// So we don't need a title asset for every UI Preset.
			CombatTrackerLib::centeredImage(
				m_titleTexture, ImGui::GetStyle().Colors[ImGuiCol_Text]
			);

			/*
            Options popup for application-wide settings.
            TODO: One thing to do is actually save these settings somewhere so they are preserved between runs.
            */
			const auto viewportSize{ ImGui::GetMainViewport()->Size };
			ImGui::SetNextWindowPos(
				ImGui::GetMainViewport()->GetCenter(),
				ImGuiCond_Appearing,
				ImVec2(0.5f, 0.5f)
			);

			// Also default to a sensible minimum popup size.
			ImGui::SetNextWindowSize(
				ImVec2(
					std::min(640.0f, viewportSize.x * 0.80f),
					std::min(420.0f, viewportSize.y * 0.80f)
				),
				ImGuiCond_Appearing
			);
			ImGui::SetNextWindowSizeConstraints(
				ImVec2(
					std::min(360.0f, viewportSize.x),
					std::min(240.0f, viewportSize.y)
				),
				viewportSize
			);

			// The title bar is hidden so the panel can draw its own header.
			if (CombatTrackerLib::ScopedPopupModal optionsPopup{
					"Options Popup",
					nullptr,
					ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove }) {

				{
					CombatTrackerLib::ScopedFont headerFont{
						nullptr, ImGui::GetStyle().FontSizeBase * 1.35f
					};
					ImGui::TextUnformatted(
						m_localeProviderPointer
							->getText("/scenes/mainMenu/optionsPopup/header")
							.data()
					);
				}
				ImGui::Separator();

				// The footer holds a separator and the close button, the rest of the popup contains the two panels.
				const auto footerHeight{
					CombatTrackerLib::reservedFooterHeight(1)
				};
				const auto bodyHeight{ ImGui::GetContentRegionAvail().y -
									   footerHeight };

				// Category list.
				const auto categoryWidth{ std::min(
					ImGui::GetFontSize() * 9.0f,
					ImGui::GetContentRegionAvail().x * 0.35f
				) };
				{
					CombatTrackerLib::ScopedChild optionsCategories{
						"Options Categories",
						ImVec2(categoryWidth, bodyHeight),
						ImGuiChildFlags_Borders
					};
					const auto categoryNames{
						m_localeProviderPointer->getTextList(
							"/scenes/mainMenu/optionsPopup/categories"
						)
					};
					for (size_t i{}; i < categoryNames.size(); ++i) {
						const auto category{
							static_cast<CombatTrackerLib::OptionsCategory>(i)
						}; // Cast is needed here so that we actually store the enumerator type.
						if (ImGui::Selectable(
								categoryNames[i].data(),
								m_selectedOptionsCategory == category
							)) {
							m_selectedOptionsCategory = category;
						}
					}
				}

				ImGui::SameLine();

				// Controls belonging to the selected category.
				{
					CombatTrackerLib::ScopedChild optionsContent{
						"Options Content",
						ImVec2(0.0f, bodyHeight),
						ImGuiChildFlags_Borders
					};
					if (m_selectedOptionsCategory ==
						CombatTrackerLib::OptionsCategory::GENERAL) {
						ImGui::SeparatorText(
							m_localeProviderPointer
								->getText(
									"/scenes/mainMenu/optionsPopup/language"
								)
								.data()
						);

						ImGui::TextUnformatted(
							m_localeProviderPointer
								->getText("/scenes/mainMenu/optionsPopup/"
										  "chooseLanguage")
								.data()
						);
						ImGui::SetNextItemWidth(-1.0f);

						const auto currentLocaleCode{
							m_applicationPointer->getOptionsHandler()
								.getOption<std::string>(
									CombatTrackerLib::OptionsCategory::GENERAL,
									"language"
								)
						};
						if (CombatTrackerLib::ScopedListBox languages{
								"##ChooseLanguage" }) {
							for (const auto& locale :
								 m_localeProviderPointer->getAvailableLocales(
								 )) {
								if (ImGui::Selectable(
										locale.endonym.c_str(),
										locale.code == currentLocaleCode
									)) {
									pendingLocaleCode = locale.code;
								}
							}
						}
					}
					if (m_selectedOptionsCategory ==
						CombatTrackerLib::OptionsCategory::APPEARANCE) {
						ImGui::SeparatorText(
							m_localeProviderPointer
								->getText("/scenes/mainMenu/optionsPopup/theme")
								.data()
						);

						// Hide the label of the listbox and put it above the widget, so the listbox can take the full panel width.
						ImGui::TextUnformatted(
							m_localeProviderPointer
								->getText(
									"/scenes/mainMenu/optionsPopup/chooseTheme"
								)
								.data()
						);
						ImGui::SetNextItemWidth(-1.0f
						); // -1.0f passed to the function means that the item spans the entire width.

						const auto currentThemeName{
							m_applicationPointer->getOptionsHandler()
								.getOption<std::string>(
									CombatTrackerLib::OptionsCategory::
										APPEARANCE,
									"theme"
								)
						};
						if (CombatTrackerLib::ScopedListBox themes{
								"##ChooseTheme" }) {
							for (const auto& themeName :
								 m_applicationPointer->getThemeProvider()
									 .getThemeNames()) {
								if (ImGui::Selectable(
										themeName.c_str(),
										themeName == currentThemeName
									)) {
									m_applicationPointer->setTheme(themeName
									); // This function early returns already so this is fine.
								}
							}
						}

						ImGui::SeparatorText(
							m_localeProviderPointer
								->getText("/scenes/mainMenu/optionsPopup/window"
								)
								.data()
						);

						auto& optionsHandler{
							m_applicationPointer->getOptionsHandler()
						};

						bool isVsyncEnabled{ optionsHandler.getOption<bool>(
							CombatTrackerLib::OptionsCategory::APPEARANCE,
							"vsyncEnabled"
						) };
						if (ImGui::Checkbox(
								m_localeProviderPointer
									->getText(
										"/scenes/mainMenu/optionsPopup/vsync"
									)
									.data(),
								&isVsyncEnabled
							)) {
							m_applicationPointer->setWindowVsync(isVsyncEnabled
							);
						}

						bool isFullscreenEnabled{
							optionsHandler.getOption<bool>(
								CombatTrackerLib::OptionsCategory::APPEARANCE,
								"fullscreenEnabled"
							)
						};
						if (ImGui::Checkbox(
								m_localeProviderPointer
									->getText("/scenes/mainMenu/optionsPopup/"
											  "fullscreen")
									.data(),
								&isFullscreenEnabled
							)) {
							m_applicationPointer->setWindowFullscreen(
								isFullscreenEnabled
							);
						}

						bool isResizable{ optionsHandler.getOption<bool>(
							CombatTrackerLib::OptionsCategory::APPEARANCE,
							"isResizable"
						) };
						if (ImGui::Checkbox(
								m_localeProviderPointer
									->getText("/scenes/mainMenu/optionsPopup/"
											  "resizable")
									.data(),
								&isResizable
							)) {
							m_applicationPointer->setWindowResize(isResizable);
						}
					}
					// TODO: Update this when more categories get added.
				}

				ImGui::Separator();

				// A modal ignores both outside clicks and Escape, so this is the only way out.
				if (CombatTrackerLib::alignedButton(
						m_localeProviderPointer
							->getText("/scenes/mainMenu/optionsPopup/close")
							.data(),
						1.0f
					)) {
					ImGui::CloseCurrentPopup();
				}
			}
		}

		// Encounter Creation window.
		{
			CombatTrackerLib::ScopedWindow windowTitle{
				m_localeProviderPointer->getText(
					"/scenes/mainMenu/encounterCreation/windowTitle"
				)
			};
			ImGui::TextWrapped(
				"%s",
				m_localeProviderPointer
					->getText(
						"/scenes/mainMenu/encounterCreation/creatureManagement"
					)
					.data()
			);
			CombatTrackerLib::renderBulletList(
				m_localeProviderPointer->getTextList(
					"/scenes/mainMenu/encounterCreation/"
					"creatureManagementBullets"
				)
			);
			ImGui::TextWrapped(
				"%s",
				m_localeProviderPointer
					->getText(
						"/scenes/mainMenu/encounterCreation/encounterManagement"
					)
					.data()
			);
			CombatTrackerLib::renderBulletList(
				m_localeProviderPointer->getTextList(
					"/scenes/mainMenu/encounterCreation/"
					"encounterManagementBullets"
				)
			);
			if (CombatTrackerLib::alignedButton(
					m_localeProviderPointer
						->getText(
							"/scenes/mainMenu/encounterCreation/createButton"
						)
						.data(),
					0.5f
				)) {
				m_applicationPointer->getSceneManager().queueSceneTransition(
					this,
					std::make_unique<EncounterCreationScene>(
						m_applicationPointer
					)
				);
			}
		}

		// Run Encounter window.
		{
			CombatTrackerLib::ScopedWindow windowTitle{
				m_localeProviderPointer->getText(
					"/scenes/mainMenu/runEncounter/windowTitle"
				)
			};
			ImGui::TextWrapped(
				"%s",
				m_localeProviderPointer
					->getText("/scenes/mainMenu/runEncounter/preparationPhase")
					.data()
			);
			CombatTrackerLib::renderBulletList(
				m_localeProviderPointer->getTextList(
					"/scenes/mainMenu/runEncounter/preparationBullets"
				)
			);
			ImGui::TextWrapped(
				"%s",
				m_localeProviderPointer
					->getText("/scenes/mainMenu/runEncounter/runningEncounters")
					.data()
			);
			CombatTrackerLib::renderBulletList(
				m_localeProviderPointer->getTextList(
					"/scenes/mainMenu/runEncounter/runningBullets"
				)
			);
			ImGui::TextWrapped(
				"%s",
				m_localeProviderPointer
					->getText(
						"/scenes/mainMenu/runEncounter/finishingEncounters"
					)
					.data()
			);
			CombatTrackerLib::renderBulletList(
				m_localeProviderPointer->getTextList(
					"/scenes/mainMenu/runEncounter/finishingBullets"
				)
			);

			// TODO: Complete the scene transition.
			if (CombatTrackerLib::alignedButton(
					m_localeProviderPointer
						->getText("/scenes/mainMenu/runEncounter/selectButton")
						.data(),
					0.5f
				)) {
			}
		}

		// Completed Encounters window.
		CombatTrackerLib::ScopedWindow windowTitle{
			m_localeProviderPointer->getText(
				"/scenes/mainMenu/completedEncounters/windowTitle"
			)
		};
		CombatTrackerLib::renderBulletList(m_localeProviderPointer->getTextList(
			"/scenes/mainMenu/completedEncounters/bullets"
		));

		// TODO: Complete the scene transition.
		if (CombatTrackerLib::alignedButton(
				m_localeProviderPointer
					->getText(
						"/scenes/mainMenu/completedEncounters/browseButton"
					)
					.data(),
				0.5f
			)) {
		}

		// The locale change should be deferred here, as the change invalidates the previous view maps, leaving the string_views dangling.
		// This is kind of fragile and I don't really like it that much, consider changing.
		if (pendingLocaleCode != std::nullopt) {
			m_applicationPointer->setLanguage(*pendingLocaleCode);
		}
	}
	void MainMenuScene::renderScene() {}
}
