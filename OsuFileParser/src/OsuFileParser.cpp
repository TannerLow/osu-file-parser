#include "OsuFileParser.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include "Profiler.h"
#include <charconv>

#define ERROR_INT -0xBAD'123
#define ERROR_DOUBLE -DBL_MAX

namespace ofp {

	bool readFile(const fs::path& filepath) {
        auto start = std::chrono::steady_clock::now();

		std::ifstream file(filepath, std::ios::binary | std::ios::ate);

        if (!file) {
            std::cout << "Failed to open file\n";
            return false;
        }

        std::string contents;

        // Seek to end to get file size
        contents.resize(file.tellg()); // resize string to fit file
        file.seekg(0, std::ios::beg);

        // Read entire file into string
        file.read(&contents[0], contents.size());

        auto end = std::chrono::steady_clock::now();

        auto timeElapsed = end - start;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();
        long long timeElapsedMilli = std::chrono::duration_cast<std::chrono::milliseconds>(timeElapsed).count();
        long long timeElapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeElapsed).count();

        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << contents.size()  << " bytes in " << timeElapsedMicro << "us" << std::endl;
        std::cout << "Parsed in " << contents.size() << " bytes in " << timeElapsedMilli << "ms" << std::endl;
        std::cout << "Parsed in " << contents.size() << " bytes in " << timeElapsedSeconds << "s" << std::endl;

        return true;
	}

    bool readLines1(const fs::path& filepath) {
        auto startTime = std::chrono::steady_clock::now();

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cout << "Failed to open file\n";
            return false;
        }

        // Get file size and allocate
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string data(size, '\0');
        file.read(data.data(), size);

        std::vector<std::string> lines;
        lines.reserve(8192);

        size_t start = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == '\n') {
                size_t len = (i > start && data[i - 1] == '\r') ? i - start - 1 : i - start;
                lines.emplace_back(data.substr(start, len));
                start = i + 1;
            }
        }

        if (start < data.size()) { // last line (no trailing newline)
            lines.emplace_back(data.substr(start));
        }

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();
        
        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;

        return true;
    }

    bool readLines2(const fs::path& filepath) {
        auto startTime = std::chrono::steady_clock::now();

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cout << "Failed to open file\n";
            return false;
        }

        // Get file size
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read entire file into memory
        std::string data(size, '\0');
        file.read(&data[0], size);

        std::vector<std::string> lines;
        lines.reserve(8192);

        const char* start = data.data();
        const char* end = start + data.size();
        const char* line_start = start;

        for (const char* p = start; p != end; ++p) {
            if (*p == '\n') {
                const char* line_end = p;
                if (line_end > line_start && *(line_end - 1) == '\r')
                    --line_end; // handle CRLF

                lines.emplace_back(line_start, line_end);
                line_start = p + 1;
            }
        }

        if (line_start < end)
            lines.emplace_back(line_start, end); // last line

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();

        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;

        return true;
    }

    bool readLines3(const fs::path& filepath) {
        auto startTime = std::chrono::steady_clock::now();

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cout << "Failed to open file\n";
            return false;
        }

        // Read entire file
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::string data(size, '\0');
        file.read(data.data(), size);

        std::vector<std::string_view> lines;
        lines.reserve(8192);

        const char* start = data.data();
        const char* end = start + data.size();
        const char* line_start = start;

        for (const char* p = start; p != end; ++p) {
            if (*p == '\n') {
                const char* line_end = p;
                if (line_end > line_start && *(line_end - 1) == '\r') {
                    --line_end; // Handle CRLF
                }

                lines.emplace_back(line_start, line_end - line_start);
                line_start = p + 1;
            }
        }

        if (line_start < end) {
            lines.emplace_back(line_start, end - line_start);
        }

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();

        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;

        return true;
    }

    // Helper: emulate std::string_view::starts_with for C++17
    bool startsWith(std::string_view sv, std::string_view prefix) {
        return sv.size() >= prefix.size() &&
            sv.compare(0, prefix.size(), prefix) == 0;
    }

    void trimFront(std::string_view& sv) {
        const char* p = sv.data();
        const char* end = p + sv.size();

        while (p < end) {
            char c = *p;
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                break;
            }
            ++p;
        }

        sv.remove_prefix(static_cast<size_t>(p - sv.data()));
    }

    void trimBack(std::string_view& sv) {
        const char* begin = sv.data();
        const char* p = begin + sv.size();

        while (p > begin) {
            char c = *(p - 1);
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                break;
            }
            --p;
        }

        sv.remove_suffix(static_cast<size_t>((begin + sv.size()) - p));
    }

    OsuFile parseOsuLines_sections(std::vector<std::string_view>& lines, long long& timeElapsedMicro) {
        PROFILE_SCOPE(__FUNCTION__);
        
        int sections = 0;
        for (auto& line : lines) {
            trimFront(line);
            
            // skip comments
            if (line.size() > 2 and line[0] == '/' and line[1] == '/') {
                continue;
            }
        
            trimBack(line);
            
            // check for section header
            if (line.size() > 2 and line.front() == '[' and line.back() == ']') {
                sections++;
            }
        }

        OsuFile osuFile;
        return osuFile;
    }

    int stringToInt(const std::string_view& sv) {
        int value = 0;

        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        if (ec != std::errc()) {
            return ERROR_INT;
        }

        return value;
    }

    double stringToDouble(const std::string_view& sv) {
        double value = 0.0;

        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        if (ec != std::errc()) {
            return ERROR_DOUBLE;
        }

        return value;
    }

    int stringIntToLane(const std::string_view& sv, int keyMode) {
        int x = stringToInt(sv);
        if (x == ERROR_INT) {
            return ERROR_INT;
        }

        int column = x * keyMode / 512;

        return std::max(0, std::min(column, keyMode-1));
    }

    std::vector<std::string_view> split(const std::string_view& str, char delim) {
        std::vector<std::string_view> parts;
        parts.reserve(10);

        size_t start = 0;
        while (true) {
            size_t pos = str.find(delim, start);
            if (pos == std::string_view::npos) {
                parts.emplace_back(str.substr(start));
                break;
            }
            parts.emplace_back(str.substr(start, pos - start));
            start = pos + 1;
        }

        return parts;
    }

    bool containsCaseInsensitive(const std::string_view& text, const std::string_view& sub) {
        auto it = std::search(
            text.begin(), text.end(),
            sub.begin(), sub.end(),
            [](unsigned char a, unsigned char b) {
                return std::tolower(a) == std::tolower(b);
            });

        return it != text.end();
    }

    OsuBeatmap parseOsuLines_noHitObjects(std::vector<std::string_view>& lines) {
        PROFILE_SCOPE(__FUNCTION__);

        enum Section {
            General, 
            Editor, 
            Metadata, 
            Difficulty, 
            Events, 
            TimingPoints, 
            HitObjects,
            Colours,
            N_A
        };

        OsuBeatmap osuBeatmap;

        constexpr std::string_view fileFormatHeader = "osu file format v";
        Section currentSection = N_A;
        bool backgroundFound = false;

        for (auto& line : lines) {
            trimFront(line);

            // skip comments
            if (line.size() > 2 and line[0] == '/' and line[1] == '/') {
                continue;
            }

            trimBack(line);

            if (line.empty()) {
                continue;
            }

            // check for section header
            if (line.size() > 2 and line.front() == '[' and line.back() == ']') {
                std::string_view section = line.substr(1, line.size() - 2);
                if (section == "General") {
                    currentSection = General;
                }
                else if (section == "Editor") {
                    currentSection = Editor;
                }
                else if (section == "Metadata") {
                    currentSection = Metadata;
                }
                else if (section == "Difficulty") {
                    currentSection = Difficulty;
                }
                else if (section == "Events") {
                    currentSection = Events;
                }
                else if (section == "TimingPoints") {
                    currentSection = TimingPoints;
                }
                else if (section == "HitObjects") {
                    currentSection = HitObjects;
                }
                else if (section == "Colours") {
                    currentSection = Colours;
                }
                else {
                    currentSection = N_A;
                }

                continue;
            }

            switch (currentSection) {
            
            case N_A: {
                size_t found = line.find(fileFormatHeader);
                if (found != line.npos) {
                    // NOTE currently not used/stored, may need later for skipping old versions
                    std::string_view version = line.substr(found + fileFormatHeader.size());
                }
                break;
            }
            case General: {
                size_t pos = line.find(':'); 
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);
                
                    if (key == "AudioFilename") {
                        osuBeatmap.audioFilename = value;
                    }
                    else if (key == "PreviewTime") {
                        osuBeatmap.previewTime = stringToInt(value);
                        if (osuBeatmap.previewTime == ERROR_INT) {
                            osuBeatmap.previewTime = false;
                            return osuBeatmap;
                        }
                    }
                    else if (key == "Mode") {
                        if (value != "3") {
                            osuBeatmap.valid = false;
                            return osuBeatmap;
                        }
                    }
                }
                break;
            }
            case Metadata: {
                size_t pos = line.find(':');
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);

                    if (key == "Title") {
                        osuBeatmap.title = value;
                    }
                    else if (key == "TitleUnicode") {
                        osuBeatmap.titleUnicode = value;
                    }
                    else if (key == "Artist") {
                        osuBeatmap.artist = value;
                    }
                    else if (key == "ArtistUnicode") {
                        osuBeatmap.artistUnicode = value;
                    }
                    else if (key == "Creator") {
                        osuBeatmap.creator = value;
                    }
                    else if (key == "Version") {
                        osuBeatmap.version = value;
                    }
                    else if (key == "Source") {
                        osuBeatmap.source = value;
                    }
                    else if (key == "Tags") {
                        osuBeatmap.tags = value;
                    }
                    else if (key == "BeatmapID") {
                        osuBeatmap.id = value;
                    }
                    else if (key == "BeatmapSetID") {
                        osuBeatmap.setId = value;
                    }
                }
                break;
            }
            case Difficulty: {
                size_t pos = line.find(':');
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);
                
                    if (key == "CircleSize") {
                        osuBeatmap.keyMode = stringToInt(value);
                        if (osuBeatmap.keyMode == ERROR_INT) {
                            osuBeatmap.keyMode = false;
                            return osuBeatmap;
                        }
                    }
                }
                break;
            }
            case Events: {
                if (not backgroundFound &&
                    (containsCaseInsensitive(line, "png") ||
                    containsCaseInsensitive(line, "jpg") ||
                    containsCaseInsensitive(line, "jpeg"))
                ) {
                    std::vector<std::string_view> values = split(line, ',');

                    if (values.size() >= 2 and values[0] == "0") {
                        std::string background(values[2]);
                        background.erase(std::remove(background.begin(), background.end(), '\"'), background.end());
                        backgroundFound = true;
                    }
                }
                break;
            }
            case TimingPoints: {
                std::vector<std::string_view> values = split(line, ',');

                if (values.size() == 8) {
                    OsuTimingPoint timingPoint;
                    timingPoint.time = stringToInt(values[0]);
                    timingPoint.beatLength = stringToInt(values[1]);
                    timingPoint.volume = stringToInt(values[2]);

                    if (timingPoint.time == ERROR_INT or timingPoint.beatLength == ERROR_DOUBLE or timingPoint.volume == ERROR_INT) {
                        osuBeatmap.valid = false;
                        return osuBeatmap;
                    }

                    osuBeatmap.timingPoints.emplace_back(timingPoint);
                }
                break;
            }

            }
        }

        osuBeatmap.valid = true;
        return osuBeatmap;
    }

    OsuBeatmap parseOsuLines(std::vector<std::string_view>& lines) {
        PROFILE_SCOPE(__FUNCTION__);

        enum Section {
            General,
            Editor,
            Metadata,
            Difficulty,
            Events,
            TimingPoints,
            HitObjects,
            Colours,
            N_A
        };

        OsuBeatmap osuBeatmap;

        constexpr std::string_view fileFormatHeader = "osu file format v";
        Section currentSection = N_A;
        bool backgroundFound = false;
        bool keyModeFound = false;

        for (auto& line : lines) {
            trimFront(line);

            // skip comments
            if (line.size() > 2 and line[0] == '/' and line[1] == '/') {
                continue;
            }

            trimBack(line);

            if (line.empty()) {
                continue;
            }

            // check for section header
            if (line.size() > 2 and line.front() == '[' and line.back() == ']') {
                std::string_view section = line.substr(1, line.size() - 2);
                if (section == "General") {
                    currentSection = General;
                }
                else if (section == "Editor") {
                    currentSection = Editor;
                }
                else if (section == "Metadata") {
                    currentSection = Metadata;
                }
                else if (section == "Difficulty") {
                    currentSection = Difficulty;
                }
                else if (section == "Events") {
                    currentSection = Events;
                }
                else if (section == "TimingPoints") {
                    currentSection = TimingPoints;
                }
                else if (section == "HitObjects") {
                    currentSection = HitObjects;
                    if (not keyModeFound) {
                        osuBeatmap.valid = false;
                        return osuBeatmap;
                    }
                }
                else if (section == "Colours") {
                    currentSection = Colours;
                }
                else {
                    currentSection = N_A;
                }

                continue;
            }

            switch (currentSection) {

            case N_A: {
                size_t found = line.find(fileFormatHeader);
                if (found != line.npos) {
                    // NOTE currently not used/stored, may need later for skipping old versions
                    std::string_view version = line.substr(found + fileFormatHeader.size());
                }
                break;
            }
            case General: {
                size_t pos = line.find(':');
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);

                    if (key == "AudioFilename") {
                        osuBeatmap.audioFilename = value;
                    }
                    else if (key == "PreviewTime") {
                        osuBeatmap.previewTime = stringToInt(value);
                        if (osuBeatmap.previewTime == ERROR_INT) {
                            osuBeatmap.previewTime = false;
                            return osuBeatmap;
                        }
                    }
                    else if (key == "Mode") {
                        if (value != "3") {
                            osuBeatmap.valid = false;
                            return osuBeatmap;
                        }
                    }
                }
                break;
            }
            case Metadata: {
                size_t pos = line.find(':');
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);

                    if (key == "Title") {
                        osuBeatmap.title = value;
                    }
                    else if (key == "TitleUnicode") {
                        osuBeatmap.titleUnicode = value;
                    }
                    else if (key == "Artist") {
                        osuBeatmap.artist = value;
                    }
                    else if (key == "ArtistUnicode") {
                        osuBeatmap.artistUnicode = value;
                    }
                    else if (key == "Creator") {
                        osuBeatmap.creator = value;
                    }
                    else if (key == "Version") {
                        osuBeatmap.version = value;
                    }
                    else if (key == "Source") {
                        osuBeatmap.source = value;
                    }
                    else if (key == "Tags") {
                        osuBeatmap.tags = value;
                    }
                    else if (key == "BeatmapID") {
                        osuBeatmap.id = value;
                    }
                    else if (key == "BeatmapSetID") {
                        osuBeatmap.setId = value;
                    }
                }
                break;
            }
            case Difficulty: {
                size_t pos = line.find(':');
                if (pos != std::string_view::npos) {
                    std::string_view key = line.substr(0, pos);
                    std::string_view value = line.substr(pos + 1);
                    trimFront(value);

                    if (key == "CircleSize") {
                        osuBeatmap.keyMode = stringToInt(value);
                        keyModeFound = true;
                        if (osuBeatmap.keyMode == ERROR_INT) {
                            osuBeatmap.keyMode = false;
                            return osuBeatmap;
                        }
                    }
                }
                break;
            }
            case Events: {
                if (not backgroundFound &&
                    (containsCaseInsensitive(line, "png") ||
                        containsCaseInsensitive(line, "jpg") ||
                        containsCaseInsensitive(line, "jpeg"))
                    ) {
                    std::vector<std::string_view> values = split(line, ',');

                    if (values.size() >= 2 and values[0] == "0") {
                        std::string background(values[2]);
                        background.erase(std::remove(background.begin(), background.end(), '\"'), background.end());
                        backgroundFound = true;
                    }
                }
                break;
            }
            case TimingPoints: {
                std::vector<std::string_view> values = split(line, ',');

                if (values.size() == 8) {
                    OsuTimingPoint timingPoint;
                    timingPoint.time = stringToInt(values[0]);
                    timingPoint.beatLength = stringToInt(values[1]);
                    timingPoint.volume = stringToInt(values[2]);

                    if (timingPoint.time == ERROR_INT or timingPoint.beatLength == ERROR_DOUBLE or timingPoint.volume == ERROR_INT) {
                        osuBeatmap.valid = false;
                        return osuBeatmap;
                    }

                    osuBeatmap.timingPoints.emplace_back(timingPoint);
                }
                break;
            }
            case HitObjects: {
                std::vector<std::string_view> values = split(line, ',');

                if (values.size() >= 4) {
                    OsuHitObject hitObject;
                    hitObject.lane = stringIntToLane(values[0], osuBeatmap.keyMode);
                    hitObject.startTime = stringToInt(values[2]);
                    hitObject.isLongNote = stringToInt(values[3]) & 0x80;

                    if (hitObject.isLongNote) {
                        hitObject.endTime = stringToInt(values[5]);
                        if (hitObject.endTime == ERROR_INT) {
                            osuBeatmap.valid = false;
                            return osuBeatmap;
                        }
                    }

                    if (hitObject.lane == ERROR_INT or hitObject.startTime == ERROR_INT) {
                        osuBeatmap.valid = false;
                        return osuBeatmap;
                    }

                    osuBeatmap.hitObjects.emplace_back(hitObject);
                }

                OsuHitObject hitObject;
                break;
            }

            }
        }

        osuBeatmap.valid = true;
        return osuBeatmap;
    }

    OsuFile parseOsuLinesSample(const std::vector<std::string_view>& lines) {
        auto startTime = std::chrono::steady_clock::now();

        OsuFile osu;
        Section* current = nullptr;

        for (auto line : lines) {
            // Trim whitespace (simple)
            while (!line.empty() && std::isspace(static_cast<unsigned char>(line.front())))
                line.remove_prefix(1);
            while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back())))
                line.remove_suffix(1);

            if (line.empty()) continue;

            // Detect section
            if (line.front() == '[' && line.back() == ']') {
                osu.sections.push_back({ line.substr(1, line.size() - 2), {} });
                current = &osu.sections.back();
            }
            // Detect key-value pair
            else if (auto pos = line.find(':'); pos != std::string_view::npos) {
                auto key = line.substr(0, pos);
                auto value = line.substr(pos + 1);

                // Trim spaces around key and value
                while (!key.empty() && std::isspace(static_cast<unsigned char>(key.front())))
                    key.remove_prefix(1);
                while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back())))
                    key.remove_suffix(1);
                while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())))
                    value.remove_prefix(1);
                while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())))
                    value.remove_suffix(1);

                if (current)
                    current->entries[key] = value;
            }
            // Detect header
            else if (startsWith(line, "osu file format v")) {
                osu.version = line;
            }
        }

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();

        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;

        return osu;
    }

    OsuBeatmap readOsuFile(const fs::path& filepath) {
        PROFILE_SCOPE(__FUNCTION__);

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cout << "Failed to open file\n";
            return OsuBeatmap();
        }

        // Read entire file
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string data(size, '\0');
        file.read(data.data(), size);

        std::vector<std::string_view> lines;
        lines.reserve(8192);

        const char* start = data.data();
        const char* end = start + data.size();
        const char* line_start = start;

        for (const char* p = start; p != end; ++p) {
            if (*p == '\n') {
                const char* line_end = p;
                if (line_end > line_start && *(line_end - 1) == '\r') {
                    --line_end; // Handle CRLF
                }

                lines.emplace_back(line_start, line_end - line_start);
                line_start = p + 1;
            }
        }

        if (line_start < end) {
            lines.emplace_back(line_start, end - line_start);
        }

        return parseOsuLines(lines);
    }

    std::vector<fs::path> findOsuFiles(const fs::path& directory) {
        auto startTime = std::chrono::steady_clock::now();

        std::vector<fs::path> osu_files;

        try {
            for (auto& entry : fs::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    if (path.has_extension() && path.extension() == ".osu") {
                        osu_files.push_back(path);
                    }
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cout << "Filesystem error: " << e.what() << "\n";
        }

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMilli = std::chrono::duration_cast<std::chrono::milliseconds>(timeElapsed).count();

        std::cout << "Found " << osu_files.size() << " files in " << timeElapsedMilli << "ms" << std::endl;

        return osu_files;
    }
}
