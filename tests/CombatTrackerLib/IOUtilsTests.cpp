#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string_view>

#include "Utilities/IOUtils.h"

namespace {

	// A scratch file that writes its content on construction and removes itself afterwards.
	// Shaders are read off disk, so the read path is worth covering with a real file rather than a mock.
	class ScopedTestFile {
	public:
		explicit ScopedTestFile(std::string_view content) :
			m_path{ std::filesystem::temp_directory_path() / "CombatTrackerIOUtilsTest.txt" }
		{
			std::ofstream file{ m_path };
			file << content;
		}
		ScopedTestFile(const ScopedTestFile& file) = delete;
		ScopedTestFile& operator=(const ScopedTestFile& file) = delete;
		~ScopedTestFile() {
			std::filesystem::remove(m_path);
		}
		const std::filesystem::path& getPath() const {
			return m_path;
		}
	private:
		std::filesystem::path m_path{};
	};
}

TEST(IOUtilsTests, ReadsBackEverythingThatWasWritten) {
	const ScopedTestFile file{ "#version 330 core\nvoid main() {}\n" };
	EXPECT_EQ(CombatTrackerLib::readFileToString(file.getPath()), "#version 330 core\nvoid main() {}\n");
}

TEST(IOUtilsTests, ReadsAnEmptyFileAsAnEmptyString) {
	const ScopedTestFile file{ "" };
	EXPECT_TRUE(CombatTrackerLib::readFileToString(file.getPath()).empty());
}

// A missing file is reported by an empty return value, which is what the shader constructor checks for.
TEST(IOUtilsTests, ReturnsAnEmptyStringWhenTheFileIsMissing) {
	const auto missingPath{ std::filesystem::temp_directory_path() / "CombatTrackerMissingFile.txt" };
	std::filesystem::remove(missingPath);
	EXPECT_TRUE(CombatTrackerLib::readFileToString(missingPath).empty());
}
