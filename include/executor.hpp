#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "command.hpp"

void executeExternal(const ParsedCommand& cmd);
void executePipeline(const ParsedCommand& cmd);
void setupSignalHandlers();
void cleanupZombieProcesses();

#endif  // EXECUTOR_HPP
