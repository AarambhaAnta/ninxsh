#include "jobs.hpp"

#include <algorithm>
#include <iostream>
#include <sys/wait.h>

int JobManager::addJob(pid_t pid, const std::string& command) {
    int jobId = nextJobId++;
    jobs.emplace_back(jobId, pid, command);
    return jobId;
}

void JobManager::removeJob(pid_t pid) {
    jobs.erase(
        std::remove_if(jobs.begin(), jobs.end(), [pid](const Job& job) { return job.pid == pid; }),
        jobs.end());
}

void JobManager::updateJobStatus(pid_t pid, bool isRunning, bool isStopped) {
    Job* job = findJobByPid(pid);
    if (job) {
        job->isRunning = isRunning;
        job->isStopped = isStopped;
    }
}

Job* JobManager::findJobByPid(pid_t pid) {
    for (auto& job : jobs) {
        if (job.pid == pid) {
            return &job;
        }
    }
    return nullptr;
}

Job* JobManager::findJobById(int jobId) {
    for (auto& job : jobs) {
        if (job.jobId == jobId) {
            return &job;
        }
    }
    return nullptr;
}

void JobManager::cleanupJobs() {
    int status;
    pid_t pid;

    // Check for finished jobs without blocking
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        Job* job = findJobByPid(pid);
        if (job) {
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                // Job finished, remove it
                removeJob(pid);
            }
        }
    }
}

void JobManager::printJobs() const {
    if (jobs.empty()) {
        return;
    }

    for (const auto& job : jobs) {
        std::string status;
        if (job.isStopped) {
            status = "Stopped";
        } else if (job.isRunning) {
            status = "Running";
        } else {
            status = "Done";
        }

        std::cout << "[" << job.jobId << "]  " << status << "                 " << job.command
                  << std::endl;
    }
}
