#include "core.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
//#include <thread>
#include "path_settings.h"

using namespace m2::core;

const std::string chosenFilePath = "/home/vova/chosen_server.txt";

Core::Core() : logger_(GetManagerPath("logs").append("core.log")) {
    contactManager_ = std::make_shared<ContactManager>();
    messageManager_ = std::make_shared<MessageManager>();
    loginManager_ = std::make_shared<LoginManager>();

    std::ifstream serverFileStream(chosenFilePath);
    if (serverFileStream.is_open()) {
        serverFileStream >> chosenServer_;
        logger_(SL_DEBUG) << "get the server domain from the file";
    } else {
        logger_(SL_DEBUG) << "the file does not open! the chosenServer_ is empty";
    }
}

/********* MANAGERS *******/
std::shared_ptr<ContactManager> Core::GetContactManager() { return contactManager_; }

std::shared_ptr<LoginManager> Core::GetLoginManager() { return loginManager_; }

std::shared_ptr<MessageManager> Core::GetMessageManager() { return messageManager_; }

boost::optional<m2::Error> Core::StartServerConnection(const m2::Uuid &serverGuid) {
    boost::optional<m2::Error> error;

    return error;
}

/********* CORE <--> SERVER *********/
boost::optional<m2::Error> Core::RegisterNewUser() {}

boost::optional<m2::Error> Core::Login(const m2::Uuid &clientUuid) {
    boost::optional<m2::Error> error;
    // TODO: send user uuid to the server and try to login
    return error;
}

/********* THREADS *********/
void Core::Start() {
    std::cout << "Start()" << std::endl;
    keepWorking_ = true;
    JobLoop();
}

void Core::Stop() {
    std::cout << "Stop core!" << std::endl;
    std::unique_lock<std::mutex> lock(mutex_);
    keepWorking_ = false;
    hasJob_.notify_one();
}

void Core::JobLoop() {
    while (keepWorking_) {
        std::cout << "wait loop" << std::endl;
        JobType job;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (jobQueue_.empty()) {
                std::cout << "jobs empty" << std::endl;
                hasJob_.wait(lock);
                if (!keepWorking_) return;
            }

            job = jobQueue_.front();
            jobQueue_.pop();
        }
        std::cout << "call job" << std::endl;
        job(*this);
    }
}

void Core::PushJob(JobType job, std::string &&jobname) {
    std::cout << "      push job" << std::endl;
    logger_(SL_DEBUG) << "push job with name " + jobname;
    std::unique_lock<std::mutex> lock(mutex_);
    jobQueue_.push(job);
    hasJob_.notify_one();
}

/***********SERVER DOMAIN*****************/
void Core::SetChosenServer(const std::string &serverDomain) {
    chosenServer_ = serverDomain;
    std::ofstream serveFileStream(chosenFilePath);
    if (serveFileStream.is_open()) {
        serveFileStream << chosenServer_;
    } else {
        logger_(SL_ERROR) << "the file does not open! the chosen server was not saved!";
    }
}

/**********HTTP INTERFACE*****************/
bool Core::InitHttpConnection(const std::string &serverDomain) {
    std::string validServerDomain = serverDomain.empty() ? chosenServer_ : serverDomain;
    if (httpConnection == nullptr) httpConnection = httpClient.connect(validServerDomain);
    if (httpConnection != nullptr) {
        chosenServer_ = serverDomain;
    } else {
        logger_(SL_ERROR) << "The Connection does not exist!";
    }
    return httpConnection != nullptr;
}
