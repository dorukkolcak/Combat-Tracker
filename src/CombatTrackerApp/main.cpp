#include "../CombatTrackerLib/App.h"
int main() {
	CombatTrackerLib::WindowSpecification windowSpec(1000, 800, "test", true, true);
	CombatTrackerLib::AppSpecification appSpec("CombatTracker", windowSpec);
	CombatTrackerLib::App application(appSpec);
	// TODO: Don't forget to add initial scenes here.
	application.run();
}