/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   background_png;
    const int            background_pngSize = 18347;

    extern const char*   maximilianTest_xccheckout;
    const int            maximilianTest_xccheckoutSize = 1501;

    extern const char*   maximilianTest_10_8_xccheckout;
    const int            maximilianTest_10_8_xccheckoutSize = 1511;

    extern const char*   contents_xcworkspacedata;
    const int            contents_xcworkspacedataSize = 159;

    extern const char*   project_pbxproj;
    const int            project_pbxprojSize = 17839;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
