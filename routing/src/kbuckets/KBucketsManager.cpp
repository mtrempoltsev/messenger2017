#include <algorithm>
#include "kbuckets/KBucketsManager.h"

namespace m2 {
namespace routing {


    KBucketsManager::KBucketsManager(const NodeInfo &nodeInfo)
    : ourNodeInfo(nodeInfo)
    {
        KBucket initialKBucket;
        initialKBucket.insert(nodeInfo);
        intervalToBucket.insert(std::pair<int, KBucket>(0, initialKBucket));
    }

    void KBucketsManager::insert(const NodeInfo &newNodeInfo) {
//        int xorResult = KBucketsTools::calculateXor(ourNodeInfo, newNodeInfo); //TODO
        int xorResult = 1; //some power of 2
        int bucketIndex = getIntervalInMap(xorResult);

        if (bucketIndex != -1) //think about situation when bucketIndex == -1
        {
            auto bucket = intervalToBucket[bucketIndex];

            auto bucketList = bucket.known(); //TODO replace by KBucket method
            bool found = (std::find(bucketList.begin(), bucketList.end(), newNodeInfo) != bucketList.end());
            if (found) {
                //TODO call KBucket::moveToTop(newNodeInfo)
                return;
            }

            int K = Config::getK();
            if (bucket.knownCnt() < K) {
                bucket.insert(newNodeInfo);
            } else { //bucket is full
                //TODO replace by KBucket method
                bool foundOurs = (std::find(bucketList.begin(), bucketList.end(), ourNodeInfo) != bucketList.end());
                if (foundOurs) { //this bucket contains ourNodeInfo
                    split(bucketIndex, newNodeInfo);
                    return;
                } else {
                    auto lastConnectedNodeInfo = bucket.oldest();
                    //TODO ping lastConnectedNodeInfo, do appropriate actions when it would answer on ping or not
                }

            }
        }
    }

    void KBucketsManager::getNeighbours(const NodeInfo &nodeInfo) const
    {

    }

    KBucketsManager::KBucketsManager()
    { //TODO remove it later, it's just for build

    }

    int KBucketsManager::getIntervalInMap(int xorResult)
    {
        std::list<int> smallerKeys;

        std::for_each(intervalToBucket.begin(), intervalToBucket.end(),
                      [xorResult, &smallerKeys](std::pair<int, KBucket> keyToValue) {
                          int index = keyToValue.first;
                          if (index < xorResult) {
                              smallerKeys.push_back(index);
                          }
                      });

        if (smallerKeys.size() == 0) //actually we shouldn't face this situation...
            return -1;

        auto result = std::max_element(smallerKeys.begin(), smallerKeys.end());
        return *result;
    }

    void KBucketsManager::setNodeInfo(const NodeInfo &nodeInfo) {
        ourNodeInfo = nodeInfo;
    }

    void KBucketsManager::split(int interval, const NodeInfo &newNodeInfo) {

    }

} // namespace routing
} // namespace m2

