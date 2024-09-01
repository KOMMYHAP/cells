#pragma once
#include "base_widget.h"

struct SDL_Renderer;

class CustomRenderWidget : public BaseWidget {
public:
    virtual void RenderWidget(SDL_Renderer& renderer) = 0;
};