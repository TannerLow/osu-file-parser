#include "Profiler.h"
#include <iostream>
#include <thread>

// =======================
// === Profiler.cpp ======
// =======================

ProfileNode::ProfileNode(std::string name, ProfileNode* parent)
    : name(std::move(name)), parent(parent) {}

// thread-local definitions
thread_local std::stack<ProfileNode*> Profiler::stack_;
thread_local std::unordered_map<ProfileNode*, Profiler::Clock::time_point> Profiler::startTimes_;

Profiler::Profiler()
    : root(std::make_unique<ProfileNode>("root")) {}

Profiler& Profiler::instance() {
    static Profiler p;
    return p;
}

void Profiler::begin(const std::string& name) {
    auto now = Clock::now();

    auto& stack = getStack();
    ProfileNode* parent = stack.empty() ? root.get() : stack.top();
    auto* node = findOrCreateChild(parent, name);

    node->callCount++;
    stack.push(node);
    startTimes()[node] = now;
}

void Profiler::end() {
    auto now = Clock::now();
    auto& stack = getStack();

    if (stack.empty()) {
        return;
    }

    ProfileNode* node = stack.top();
    stack.pop();

    auto start = startTimes()[node];
    node->totalTimeMs += std::chrono::duration<double, std::milli>(now - start).count();
}

void Profiler::report() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "=== PROFILING RESULTS ===\n";
    printNode(root.get(), 0);

    root = std::make_unique<ProfileNode>("root");

    // clear stacks
    auto& stack = getStack();
    while (not stack.empty()) {
        stack.pop();
    }
    startTimes().clear();
}

std::stack<ProfileNode*>& Profiler::getStack() { return stack_; }
std::unordered_map<ProfileNode*, Profiler::Clock::time_point>& Profiler::startTimes() { return startTimes_; }

ProfileNode* Profiler::findOrCreateChild(ProfileNode* parent, const std::string& name) {
    for (auto* child : parent->children) {
        if (child->name == name) {
            return child;
        }
    }
    auto* node = new ProfileNode(name, parent);
    parent->children.push_back(node);
    return node;
}

void Profiler::printNode(ProfileNode* node, int indent) {
    if (node->name != "root") {
        std::cout << std::string(indent * 2, ' ') 
            << node->name
            << " - " << node->totalTimeMs << " ms ("
            << node->callCount << " calls)\n";
    }
    for (auto* child : node->children) {
        printNode(child, indent + 1);
    }
}

// === ProfileScope ===

ProfileScope::ProfileScope(const std::string& name) {
    Profiler::instance().begin(name);
}

ProfileScope::~ProfileScope() {
    stop();
}

void ProfileScope::stop() {
    if (active) {
        Profiler::instance().end();
    }
}
