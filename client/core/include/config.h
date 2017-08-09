#pragma once

#include <map>
#include <boost/property_tree/ptree.hpp>

#include "guid.h"

namespace m2 {
    namespace core {

        class Config final {
        public:
            //property names
            enum class PropertyName
            {
                ClientGuid,
                ServerGuid,
				FilesFolderPath,
				KeysFilePath,
				HistoryFolderPath
            };

            Config();

            bool Init(const std::string & fileName);

            //getters
            Uuid GetClientGuid() const;
            Uuid GetServerGuid() const;
			std::string GetFilesFolderPath() const;
			std::string GetKeysFilePath() const;
			std::string GetHistoryFolderPath() const;
            std::string GetProperty(const PropertyName & property) const;

            //setters
            bool SetClientGuid(const Uuid & uuid);
            bool SetServerGuid(const Uuid & uuid);
			bool SetFilesFolderPath(const std::string & filesFolderPath);
			bool SetKeysFilePath(const std::string & keysFilePath);
			bool SetHistoryFolderPath(const std::string & historyFolderPath);
            bool SetDataByKey(const PropertyName & property, const std::string & data);
            //others methods
            bool HasChanges() { return hasChanges_; }
            void CommitChanges(const std::string & fileName);


        private:
            bool hasChanges_;
            boost::property_tree::ptree propertiesTree_;
            static std::map<PropertyName, std::string> propertyNameMap_;

            bool CheckPropertyTree();

        };

    }
}
