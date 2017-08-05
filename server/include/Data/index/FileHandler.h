#ifndef M2_SERVER_FILEHANDLER_H
#define M2_SERVER_FILEHANDLER_H

#include "CashManager.hpp"
#include "Data/stdafx.h"



namespace indices {


    /**
     * the class
     * opens files, relate by it's root
     * and save it's data in structure
     *
     * std::shared_ptr<
     *      std::pair<
     *              std::string filename
     *              std::string filedata
     * > >
     *
     * to have guaranty, of file existing
     * it's required to save the shared_ptr
     *
     * because in case of normalize function
     * call the oldest cashed files with no
     * extern refs will be removed from cash.
     */
    class AFileHandler
    //    : boost::noncopyable
    {
    public:

        typedef std::string             LRoot;
        typedef std::string             LName;
        typedef std::string             LData;

        class UFile
            : public boost::enable_shared_from_this<UFile>
            , public std::pair<LName, LData> {};
        typedef std::pair<LName, LData> LFile;
        typedef TCashManager<LFile>     LCash;

        using LFilePtr  = std::shared_ptr<LFile>;
        using LCFilePtr = const LFilePtr;

    public: /***********| Construction |*********/

        AFileHandler(const std::string&  Root, size_t cash_length);
        AFileHandler(      std::string&& Root, size_t cash_length);

    public: /***********| Interface |************/

        // if file doesn't exist - nullptr
        LCFilePtr ReadFile  (const std::string& FileName);
        LCFilePtr WriteFile (const std::string& FileName, const std::string& Data);
        int       RemoveFile(const std::string& FileName);

    protected: /********| Members |**************/

        LRoot root;
        LCash cash;

    public:

        const std::string& Root() const;

    protected: /********|  |**************/

        LFilePtr read_from_disk(const std::string& FileName);

    public: /***********| Operators |************/

        LCFilePtr operator()(const std::string& FileName);

    };


}

#endif //M2_SERVER_FILEHANDLER_H
