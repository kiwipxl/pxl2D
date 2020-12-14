# pxl2D
pxl2D is a simple 2D game engine written in c++. The aim of the project was to teach myself how to make a game engine while also learning c++.

[Simple demo on android](https://www.youtube.com/watch?v=dQBV_4cltuw)

[![Thumbnail](https://i.ytimg.com/vi/dQBV_4cltuw/hqdefault.jpg)](https://www.youtube.com/watch?v=dQBV_4cltuw)

## Features
* Can build natively on windows and on android using the android-ndk
* Window handling
* Render batching (batching as many sprites as we can before sending mesh to GPU)
* Texture sheet/atlas (multiple sprites using the same texture but at different regions)
* .TTF text rendering
* Various pre-built shaders
* Point lights
* Frame buffers
* Z ordering

## Roadmap
* Joystick support
* Animation handling
* Audio
* Pixel perfect collision
* Linux window creation
