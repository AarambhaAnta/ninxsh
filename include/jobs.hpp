#ifndef JOBS_HPP
#define JOBS_HPP

#include <string>
#include <unistd.h>
#include <vector>

struct Job {
    int jobId;
    pid_t pid;
    std::string command;
    bool isRunning;
    bool isStopped;

    Job(int id, pid_t p, const std::string& cmd)
        : jobId(id), pid(p), command(cmd), isRunning(true), isStopped(false) {}
};

class JobManager {
private:
    std::vector<Job> jobs;
    int nextJobId;

public:
    JobManager() : nextJobId(1) {}

    // Add a new background job
    int addJob(pid_t pid, const std::string& command);

    // Remove a job (when it finishes)
    void removeJob(pid_t pid);

    // Update job status
    void updateJobStatus(pid_t pid, bool isRunning, bool isStopped = false);

    // Get all jobs
    const std::vector<Job>& getJobs() const {
        return jobs;
    }

    // Find job by PID
    Job* findJobByPid(pid_t pid);

    // Find job by job ID
    Job* findJobById(int jobId);

    // Clean up finished jobs
    void cleanupJobs();

    // Print all jobs (for jobs command)
    void printJobs() const;
};

#endif  // JOBS_HPP
