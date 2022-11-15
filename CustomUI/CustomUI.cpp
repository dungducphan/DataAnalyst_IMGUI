//
// Created by dphan on 11/11/22.
//

#include <CustomUI.h>
#include <imgui.h>
#include <implot.h>
#include <ImGuiFileDialog.h>

#include <math.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

#include <TString.h>
#include <TList.h>

static std::string gROOTFileName = "";
static std::string gROOTFileNameOnly = "";
static std::string gROOTFilePath = "";
static TFile* gROOTFilePtr = nullptr;
static CustomUI::ROOTFileStruct_t gROOTTreeList;

namespace CustomUI {
    ROOTFileStruct_t TraverseROOTFile(std::string filename, std::string basepath) {
        gROOTFilePtr = new TFile(gROOTFileName.c_str(), "READ");
        ROOTFileStruct_t treeList;

        auto objectList = gROOTFilePtr->GetListOfKeys();
        for (unsigned int i = 0; i < objectList->GetSize(); i++) {
            if (!std::strncmp(objectList->At(i)->ClassName(), "TTree", 5)) continue;
            else {
                TTree* aTree = (TTree*) gROOTFilePtr->Get(objectList->At(i)->GetName());
                auto branchKeys = aTree->GetListOfBranches();
                std::vector<std::string> branchList;
                for (unsigned int j = 0; j < branchKeys->GetLast() + 1; j++) {
                    branchList.push_back(branchKeys->At(j)->GetName());
                }
                treeList.push_back(std::make_pair(objectList->At(i)->GetName(), branchList));
            }
        }

        return treeList;
    }

    void GraphUI() {
        ImGui::Begin("GraphUI");
        if (ImPlot::BeginPlot("Synthetic Data")) {
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    void RenderUI() {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        static bool file_dialog_opened = false;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Main Docking", nullptr, window_flags);
        if (!opt_padding) ImGui::PopStyleVar();
        if (opt_fullscreen) ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Windows")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();
                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Files")) {
                ImGui::MenuItem("Open ROOT file", NULL, &file_dialog_opened);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (gROOTFileName != "") {
            ImGui::Begin("Data Structure");
            if (ImGui::TreeNode(gROOTFileNameOnly.c_str())) {
                for (int i = 0; i < gROOTTreeList.size(); i++) {
                    bool open1 = ImGui::TreeNode((void*)(intptr_t)i, Form("TTree::%s", gROOTTreeList.at(i).first.c_str()));
                    if (open1) {
                        for (int j = 0; j < gROOTTreeList.at(i).second.size(); j++) {
                            bool open2 = ImGui::TreeNode((void*)(intptr_t)j, Form("TBranch::%s", gROOTTreeList.at(i).second.at(j).c_str()));
                            if (open2) ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::End();
        }

        ImGui::Begin("ROOT Viewport");
        if (gROOTFileName != "") {
            ImGui::Text(Form("ROOT file path: %s.",gROOTFileName.c_str()));
            if (gROOTTreeList.size() == 0) gROOTTreeList = TraverseROOTFile(gROOTFileName, gROOTFilePath);
        }
        ImGui::End();

        if (file_dialog_opened) {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "ROOT Selector", ".root", ".");
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    gROOTFileName = ImGuiFileDialog::Instance()->GetFilePathName();
                    gROOTFilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    gROOTFileNameOnly = ImGuiFileDialog ::Instance()->GetCurrentFileName();
                }

                // close
                ImGuiFileDialog::Instance()->Close();
                file_dialog_opened = false;
            }
        }

        ImGui::End();
    }
}
