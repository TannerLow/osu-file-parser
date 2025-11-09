#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

namespace ofp {

	struct Section {
		std::string_view name;
		std::unordered_map<std::string_view, std::string_view> entries;
	};

	struct OsuFile {
		std::string version;
		std::vector<Section> sections;
	};


	std::string testOFP(std::string input);

	bool readFile(const fs::path& filepath);
	bool readLines1(const fs::path& filepath);
	bool readLines2(const fs::path& filepath);
	bool readLines3(const fs::path& filepath);

	bool readOsuFile(const fs::path& filepath);
	//OsuFile parseOsuLines(std::vector<std::string_view>& lines);

	std::vector<fs::path> findOsuFiles(const fs::path& directory);
}
