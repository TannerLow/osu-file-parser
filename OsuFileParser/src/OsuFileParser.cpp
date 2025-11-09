#include "../include/OsuFileParser.h"
#include <fstream>
#include <iostream>
#include <chrono>

namespace ofp {

	std::string testOFP(std::string input) {
		return input + "!";
	}

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

    OsuFile parseOsuLines(std::vector<std::string_view>& lines, long long& timeElapsedMicro) {
        auto startTime = std::chrono::steady_clock::now();
        
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
        
        auto endTime = std::chrono::steady_clock::now();
        
        auto timeElapsed = endTime - startTime;
        timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();
        
        //std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        //std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;
        //std::cout << sections << " sections found" << std::endl;

        OsuFile osuFile;
        return osuFile;
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

    bool readOsuFile(const fs::path& filepath) {
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

        long long formatParseTime = 0;
        OsuFile osuFile = parseOsuLines(lines, formatParseTime);

        auto endTime = std::chrono::steady_clock::now();

        auto timeElapsed = endTime - startTime;
        long long timeElapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count();

        //std::cout << "File contents:\n" << contents;
        std::cout << "Parsed in " << lines.size() << " lines in " << timeElapsedMicro << "us" << std::endl;
        std::cout << ((double)timeElapsedMicro / lines.size()) << "us per line" << std::endl;
        std::cout << " - inner time: " << formatParseTime << "us" << std::endl;

        return true;
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
