#include "CombatTrackerLib/Scene.h"

namespace CombatTrackerLib {
	Scene::Scene(App* app)
		: m_applicationPointer{app},
		  m_localeProviderPointer{&m_applicationPointer->getLocaleProvider()} {}
}
