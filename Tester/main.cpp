#include <OFP/OsuFileParser.h>
#include <O2Profiler/Profiler.h>
#include <iostream>
#include <execution>
#include <unordered_map>

int main() {
	// C:\Users\Baio\Desktop\Games\osu!\Songs\1061136 osu!mania 7K Dan Course - Dan Phase IV\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu
	// C:\Users\Baio\Desktop\Games\osu!\Songs\985208 Co shu Nie - Zettai Zetsumei\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu
	
	ofp::readFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 1] ===" << std::endl;

	ofp::readLines1("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines1("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 2] ===" << std::endl;

	ofp::readLines2("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines2("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 3] ===" << std::endl;

	ofp::readLines3("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines3("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");
	
	std::cout << "=== [read osu file] ===" << std::endl;

	PROFILED("read small osu file",
	ofp::readOsuFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");
	);
	PROFILE_REPORT();

	PROFILED("read large osu file",
	ofp::readOsuFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	);
	PROFILE_REPORT();
	
	std::cout << "=== [find osu files] ===" << std::endl;
	
	PROFILED("find all osu filepaths",
	std::vector<fs::path> osuFilepaths = ofp::findOsuFiles("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs");
	);
	PROFILE_REPORT();

	PROFILE_BEGIN("read all last-write-times", _read_all_last_write_times);
	std::unordered_map<std::string, fs::file_time_type> lastWriteTimes;
	lastWriteTimes.reserve(osuFilepaths.size());
	for (const fs::path& osuFilepath : osuFilepaths) {
		std::error_code ec;
		fs::file_time_type lastWriteTime = fs::last_write_time(osuFilepath, ec);
		if (!ec) {
			lastWriteTimes.emplace(osuFilepath.u8string(), lastWriteTime);
		}
		else {
			lastWriteTimes.emplace(osuFilepath.u8string(), fs::file_time_type::min());
		}
	}
	PROFILE_END(_read_all_last_write_times);
	PROFILE_REPORT();

	PROFILE_BEGIN("parse all osu files (skip unmodified)", _parse_all_osu_filepaths);
	int validMaps = std::transform_reduce(
		std::execution::par,                  // run in parallel
		osuFilepaths.begin(),
		osuFilepaths.end(),
		0,
		std::plus<>(),                        // how to combine results
		[&lastWriteTimes](const fs::path& osuFilepath) {
			if (lastWriteTimes[osuFilepath.u8string()] != fs::last_write_time(osuFilepath)) {
				ofp::OsuBeatmap osuBeatmap = ofp::readOsuFile(osuFilepath);
				return osuBeatmap.valid ? 1 : 0;
			}
			else {
				return 1;
			}
		}
	);
	PROFILE_END(_parse_all_osu_filepaths);
	PROFILE_REPORT();
	std::cout << "Valid maps: " << validMaps << " / " << osuFilepaths.size() << std::endl;
	
	std::cout << "=== [parse all osu files] ===" << std::endl;

	//PROFILED("parse all osu filepaths",
	//validMaps = 0;
	//for (const fs::path& osuFilepath : osuFilepaths) {
	//	ofp::OsuBeatmap osuBeatmap = ofp::readOsuFile(osuFilepath);
	//	if (osuBeatmap.valid) {
	//		validMaps++;
	//	}
	//}
	//);
	//PROFILE_REPORT();
	//std::cout << "Valid maps: " << validMaps << " / " << osuFilepaths.size() << std::endl;

	std::cout << "=== [parse all osu files (multithreading)] ===" << std::endl;

	//PROFILED("parse all osu filepaths",
	//validMaps = std::transform_reduce(
	//	std::execution::par,                  // run in parallel
	//	osuFilepaths.begin(),
	//	osuFilepaths.end(),
	//	0,
	//	std::plus<>(),                        // how to combine results
	//	[](const fs::path& osuFilepath) {
	//		ofp::OsuBeatmap osuBeatmap = ofp::readOsuFile(osuFilepath);
	//		return osuBeatmap.valid ? 1 : 0;
	//	}
	//);
	//);
	//PROFILE_REPORT();
	//std::cout << "Valid maps: " << validMaps << " / " << osuFilepaths.size() << std::endl;

	return 0;
}
