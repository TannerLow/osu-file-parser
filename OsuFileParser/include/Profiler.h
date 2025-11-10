#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <memory>
#include <mutex>

// =======================
// === Profiler.h ========
// =======================

struct ProfileNode {
    std::string name;
    double totalTimeMs = 0.0;
    int callCount = 0;
    std::vector<ProfileNode*> children;
    ProfileNode* parent = nullptr;

    ProfileNode(std::string name, ProfileNode* parent = nullptr);
};

class Profiler {
public:
    static Profiler& instance();

    void begin(const std::string& name);
    void end();
    void report();

private:
    using Clock = std::chrono::steady_clock;

    std::unique_ptr<ProfileNode> root;
    std::mutex mtx;

    static thread_local std::stack<ProfileNode*> stack_;
    static thread_local std::unordered_map<ProfileNode*, Clock::time_point> startTimes_;

    Profiler();

    std::stack<ProfileNode*>& getStack();
    std::unordered_map<ProfileNode*, Clock::time_point>& startTimes();

    ProfileNode* findOrCreateChild(ProfileNode* parent, const std::string& name);
    void printNode(ProfileNode* node, int indent);
};

// RAII helper
struct ProfileScope {
    bool active = true;

    explicit ProfileScope(const std::string& name);
    ~ProfileScope();

    void stop();
};

// Optional convenience macro
#ifdef ENABLE_PROFILING
#define _PROFILE_SCOPE_NAME(y) profileScope_##y
#define _PROFILED(name, code, line) ProfileScope _PROFILE_SCOPE_NAME(line)(name); code; _PROFILE_SCOPE_NAME(line).stop();
#define PROFILED(name, code) _PROFILED(name, code, __LINE__)
#define PROFILE_SCOPE(name) ProfileScope _PROFILE_SCOPE_NAME(__LINE__)(name)
#define PROFILE_REPORT() Profiler::instance().report();
#else
#define PROFILED(name, code) code;
#define PROFILE_SCOPE(name) 
#define PROFILE_REPORT()
#endif