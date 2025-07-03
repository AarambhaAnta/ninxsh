#include <cassert>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "jobs.hpp"

bool test_job_management() {
    bool allTestsPassed = true;

    // Test 1: Job creation
    {
        JobManager jobManager;

        pid_t testPid = 12345;
        std::string testCommand = "sleep 10";
        int jobId = jobManager.addJob(testPid, testCommand);

        if (jobId != 1) {
            std::cerr << "Job creation failed: expected jobId 1, got " << jobId << std::endl;
            allTestsPassed = false;
        }

        const auto& jobs = jobManager.getJobs();
        if (jobs.size() != 1) {
            std::cerr << "Job creation failed: expected 1 job, got " << jobs.size() << std::endl;
            allTestsPassed = false;
        }

        if (jobs.size() > 0) {
            if (jobs[0].jobId != 1 || jobs[0].pid != testPid || jobs[0].command != testCommand ||
                !jobs[0].isRunning || jobs[0].isStopped) {
                std::cerr << "Job creation failed: job data is incorrect" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 2: Job lookup by PID
    {
        JobManager jobManager;

        pid_t pid1 = 12345;
        pid_t pid2 = 12346;
        std::string cmd1 = "sleep 10";
        std::string cmd2 = "echo hello";

        jobManager.addJob(pid1, cmd1);
        jobManager.addJob(pid2, cmd2);

        Job* job1 = jobManager.findJobByPid(pid1);
        Job* job2 = jobManager.findJobByPid(pid2);
        Job* jobNull = jobManager.findJobByPid(99999);

        if (job1 == nullptr || job2 == nullptr || jobNull != nullptr) {
            std::cerr << "Job lookup by PID failed" << std::endl;
            allTestsPassed = false;
        }

        if (job1 && job1->pid != pid1) {
            std::cerr << "Job lookup by PID failed: wrong PID returned" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 3: Job lookup by ID
    {
        JobManager jobManager;

        pid_t pid1 = 12345;
        pid_t pid2 = 12346;

        int jobId1 = jobManager.addJob(pid1, "cmd1");
        int jobId2 = jobManager.addJob(pid2, "cmd2");

        Job* jobById1 = jobManager.findJobById(jobId1);
        Job* jobById2 = jobManager.findJobById(jobId2);
        Job* jobByIdNull = jobManager.findJobById(999);

        if (jobById1 == nullptr || jobById2 == nullptr || jobByIdNull != nullptr) {
            std::cerr << "Job lookup by ID failed" << std::endl;
            allTestsPassed = false;
        }

        if (jobById1 && jobById1->jobId != jobId1) {
            std::cerr << "Job lookup by ID failed: wrong ID returned" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 4: Job status update
    {
        JobManager jobManager;

        pid_t testPid = 12345;
        jobManager.addJob(testPid, "sleep 10");

        // Update job status to stopped
        jobManager.updateJobStatus(testPid, false, true);

        Job* job = jobManager.findJobByPid(testPid);
        if (job == nullptr || job->isRunning || !job->isStopped) {
            std::cerr << "Job status update failed: stopped status not set correctly" << std::endl;
            allTestsPassed = false;
        }

        // Update job status to running
        jobManager.updateJobStatus(testPid, true, false);

        if (job == nullptr || !job->isRunning || job->isStopped) {
            std::cerr << "Job status update failed: running status not set correctly" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 5: Job removal
    {
        JobManager jobManager;

        pid_t pid1 = 12345;
        pid_t pid2 = 12346;
        pid_t pid3 = 12347;

        jobManager.addJob(pid1, "sleep 10");
        jobManager.addJob(pid2, "echo hello");
        jobManager.addJob(pid3, "cat file.txt");

        if (jobManager.getJobs().size() != 3) {
            std::cerr << "Job removal test setup failed" << std::endl;
            allTestsPassed = false;
        }

        // Remove middle job
        jobManager.removeJob(pid2);

        if (jobManager.getJobs().size() != 2 || jobManager.findJobByPid(pid2) != nullptr ||
            jobManager.findJobByPid(pid1) == nullptr || jobManager.findJobByPid(pid3) == nullptr) {
            std::cerr << "Job removal failed" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 6: Job ID increment
    {
        JobManager jobManager;

        int jobId1 = jobManager.addJob(1001, "cmd1");
        int jobId2 = jobManager.addJob(1002, "cmd2");

        if (jobId1 != 1 || jobId2 != 2) {
            std::cerr << "Job ID increment failed: initial IDs wrong" << std::endl;
            allTestsPassed = false;
        }

        jobManager.removeJob(1001);  // Remove first job

        int jobId3 = jobManager.addJob(1003, "cmd3");
        if (jobId3 != 3) {  // Should be 3, not reusing 1
            std::cerr << "Job ID increment failed: ID reuse detected" << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
