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

	struct OsuTimingPoint {
		int time;
		double beatLength;
		int volume;
	};

	struct OsuHitObject {
		bool isLongNote = false;
		int startTime;
		int endTime;
		int lane;
	};

	struct OsuBeatmap {
		bool valid = false;

		// General
		std::string audioFilename;
		int previewTime;
	
		// Metadata
		std::string title;
		std::string titleUnicode;
		std::string artist;
		std::string artistUnicode;
		std::string creator;
		std::string version;
		std::string source;
		std::string tags;
		std::string id;
		std::string setId;

		// Difficulty
		int keyMode;

		// TimingPoints
		std::vector<OsuTimingPoint> timingPoints;

		// HitObjects
		std::vector<OsuHitObject> hitObjects;
	};

	bool readFile(const fs::path& filepath);
	bool readLines1(const fs::path& filepath);
	bool readLines2(const fs::path& filepath);
	bool readLines3(const fs::path& filepath);

	OsuBeatmap readOsuFile(const fs::path& filepath);
	//OsuFile parseOsuLines(std::vector<std::string_view>& lines);

	std::vector<fs::path> findOsuFiles(const fs::path& directory);
}
