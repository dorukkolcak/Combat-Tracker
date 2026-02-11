#include "../CombatTrackerLib/App.h"
int main() {
	CombatTrackerLib::WindowSpecification windowSpec(1000, 800, "test", true);
	CombatTrackerLib::AppSpecification appSpec("CombatTracker", windowSpec);
	CombatTrackerLib::App application(appSpec);
	application.run();
}