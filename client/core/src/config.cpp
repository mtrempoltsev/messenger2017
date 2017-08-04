#include "../include/config.h"

#include <string>
#include <fstream>
#include <boost/property_tree/json_parser.hpp>

using namespace m2;
using namespace core;

using boost::property_tree::ptree;

std::map<Config::PropertyName, std::string> Config::propertyNameMap_ = {
    { Config::PropertyName::ClientGuid, "ClientGuid" },
    { Config::PropertyName::ServerGuid, "ServerGuid" },
	{ Config::PropertyName::FilesFolderPath, "FilesFolderPath" },
	{ Config::PropertyName::KeysFilePath, "KeysFilePath" },
	{Config::PropertyName::HistoryFolderPath, "HistoryFolderPath" }
}; //Fill propernty name map

Config::Config() :
    hasChanges_(false) {
} //constructor

bool Config::CheckPropertyTree() {
    bool ok = true;
    std::string emptyString = std::string();
    for (auto & it : propertyNameMap_) {
        std::string bufStr = propertiesTree_.get<std::string>(it.second, emptyString);
        if (bufStr == emptyString) ok = false;
    }//foreach propertyNameMap_
    return ok;
}//CheckPropertyTree

bool Config::Init(const std::string& fileName) {
    bool ok = true;
    try {
        std::ifstream fs;
        fs.open(fileName);
        if (fs.is_open()) {
            boost::property_tree::read_json(fs, propertiesTree_);
            ok = CheckPropertyTree();
        }//if file is open
        else {
            ok = false;
        }//file not open
    } //try read json
    catch (std::exception& ex) {
        //write a message to a log
        //do we have a log?
        ok = false;
    } //handling exception
    return ok;
}//Init

std::string Config::GetProperty(const PropertyName & property) const {
    return propertiesTree_.get<std::string>(propertyNameMap_[property], std::string());
}//GetProperty

Uuid Config::GetClientGuid() const {
    return Uuid(GetProperty(PropertyName::ClientGuid));
}//GetClientGuid

Uuid Config::GetServerGuid() const {
    return Uuid(GetProperty(PropertyName::ServerGuid));
}//GetServerGuid

std::string Config::GetFilesFolderPath() const {
	return GetProperty(PropertyName::FilesFolderPath);
}//GetFilesFolderPath

std::string Config::GetKeysFilePath() const {
	return GetProperty(PropertyName::KeysFilePath);
}//GetKeysFilePath

std::string Config::GetHistoryFolderPath() const {
	return GetProperty(PropertyName::HistoryFolderPath);
}//GetHistoryFolderPath

bool Config::SetClientGuid(const Uuid & uuid) {
    return SetDataByKey(PropertyName::ClientGuid, uuid.ToString());
}//SetClientGuid

bool Config::SetServerGuid(const Uuid & uuid) {
    return SetDataByKey(PropertyName::ServerGuid, uuid.ToString());
}//SetServerGuid

bool Config::SetFilesFolderPath(const std::string & filesFolderPath) {
	return SetDataByKey(PropertyName::FilesFolderPath, filesFolderPath);
}//SetFilesFolderPath

bool Config::SetKeysFilePath(const std::string & keysFilePath) {
	return SetDataByKey(PropertyName::KeysFilePath, keysFilePath);
}//SetKeysFilePath

bool Config::SetHistoryFolderPath(const std::string & historyFolderPath) {
	return SetDataByKey(PropertyName::HistoryFolderPath, historyFolderPath);
}//SetHistoryFolderPath

bool Config::SetDataByKey(const PropertyName & property, const std::string & data) {
    bool result = false;
    try {
        propertiesTree_.put(propertyNameMap_[property], data);
		result = true;
		hasChanges_ = true;
    } //try put data into propertiesTree
    catch (boost::property_tree::ptree_bad_data & ex) {
        //write a message to a log
        //do we have a log?
    } //handling exception
    return result;
} //SetDataByKey

void Config::CommitChanges(const std::string & fileName) {
    //1. Open File
    //2. try to write propertiesTree_ into file
    std::ofstream writer(fileName);
    if (writer.is_open()) {
        try {
            boost::property_tree::write_json(writer, propertiesTree_);
            hasChanges_ = false;
        }//try
        catch (boost::property_tree::json_parser_error & ex) {
            //write a message to a log
            //do we have a log?
        }//catch
        writer.close();
    }//if file is open
}//CommitChanges

