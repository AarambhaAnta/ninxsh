#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "command.hpp"

// Forward declaration to avoid circular dependency
class JobManager;

extern bool isShellForeground;

void executeExternal(const ParsedCommand& cmd, JobManager* jobManager = nullptr);
void executePipeline(const ParsedCommand& cmd, JobManager* jobManager = nullptr);
void setupSignalHandlers();
void cleanupZombieProcesses();
void setGlobalJobManager(JobManager* jobManager);

#endif  // EXECUTOR_HPP
