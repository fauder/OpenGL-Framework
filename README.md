# OpenGL Framework
My personal graphics framework.

It started as a project to get familiarized with the modern OpenGL API (and to compare what I learned with existing Direct3D 11 knowledge).
It will be developed further as I study new graphics techniques.

So far, I've hand-rolled my own implementations for vectors, matrices, quaternions, etc.  
Development is heavily inspired by the excellently written [Magnum](https://github.com/mosra/magnum) (for example, the type-safe angle classes `Unit`, `Degrees` & `Radians`).  
Some ideas, such as the (`Test` framework) are taken from [Cherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)'s awesome [OpenGL Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2) on YouTube.  
[Dear ImGui](https://github.com/ocornut/imgui) is used for the user interface.  
[GLFW](https://www.glfw.org/) is used for windowing & other platform-specific needs.

I do not shy away from using **some** features of modern C++ to enjoy a more pleasant coding experience, without (hopefully) getting too invested in creature comforts.  
It also helps to study template programming features of the language.

I plan on implementing DirectX 11 as the second backend in the future.
After that, Vulkan & DirectX 12 may be integrated as well.
