//
// Created by dphan on 11/11/22.
//

#ifndef ROOTIMGUI_CUSTOMUI_H
#define ROOTIMGUI_CUSTOMUI_H

#include <vector>
#include <utility>
#include <string>
#include <memory>
#include <tuple>

#include <TFile.h>
#include <TTree.h>


#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

namespace CustomUI {
    typedef std::vector<std::pair<std::string, std::vector<std::string>>> ROOTFileStruct_t;

    void GraphUI();
    void RenderUI();

    ROOTFileStruct_t TraverseROOTFile(std::string filename, std::string basepath);
}

#endif //ROOTIMGUI_CUSTOMUI_H
