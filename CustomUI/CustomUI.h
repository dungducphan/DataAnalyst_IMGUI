//
// Created by dphan on 11/11/22.
//

#ifndef ROOTIMGUI_CUSTOMUI_H
#define ROOTIMGUI_CUSTOMUI_H

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

namespace CustomUI {
    void GraphUI();
    void RenderUI();
}

#endif //ROOTIMGUI_CUSTOMUI_H
