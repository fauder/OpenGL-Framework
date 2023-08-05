# OpenGL Framework
My personal graphics framework.

Started as a project to get familiarized with the modern OpenGL API (and to compare what I learned with existing Direct3D 11 knowledge).  
It will be developed further as I study new graphics techniques.

I do not shy away from using **some** features of modern C++ to enjoy a more pleasant coding experience, without (hopefully) getting too invested in creature comforts.
It also helps study template programming features of the language, as well as other nice additions such as constexpr.

I plan on implementing DirectX 11 as the second backend in the future.
After that, Vulkan & DirectX 12 may be integrated as well.

## Implemented So Far
- Templated `Vector` (and derived `Color3` & `Color4`), `Matrix`, `Quaternion`, `Unit` (and derived `Degrees` & `Radians`) types.
- `Polar2`, `Polar3_Cylindrical`, `Polar3_Spherical` & `Polar3_Spherical_Game` types.
- Conversion methods between different rotation representations.
- Simple `Math` & `TypeTraits` namespaces (both *ongoing*).
- OpenGL abstractions such as `IndexBuffer`, `Shader`, `Texture`, `VertexBuffer`, `VertexBufferLayout`, `VertexArray`, etc. (*ongoing*)
- A `Transform` class for 3D object manipulation.
- A mostly template-based & *nearly* `virtual`-free `Test` framework to quickly test ideas/techniques (somewhat utilizing compile-time polymorphism).
- .natvis file for [visualizations](https://learn.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects?view=vs-2022) of types in the Visual Studio debugger.

## Visual Studio Debugger Visualizations
I've ~~stolen~~ investigated [Magnum](https://github.com/mosra/magnum)'s .natvis files (for Visual Studio debugger visualization of types) for a more pleasant debugging experience and made my own modifications (also [contributed to Magnum](https://github.com/mosra/magnum/pull/623)).  
The following types have custom visualizations in the debugger:
- `Vector< type, size >`: Implemented for float & double variants, with sizes 2, 3 & 4. Neatly & compactly shows the component values. Also shows if the vector is normalized.
- `Quaternion< type >`: Similar to Vector. Shows normalized info as well.
- `Matrix< type, row_size, column_size >`: Basic display support, taken straight from [Magnum](https://github.com/mosra/magnum).
- `Degrees`/`Radians`: Displays current value, as well as the counterpart `Radians`/`Degrees` value in parentheses.

## Credits
- Development is heavily inspired by [Mosra's](https://github.com/mosra) excellently written [Magnum](https://github.com/mosra/magnum) (for example, the type-safe angle classes `Unit`, `Degrees` & `Radians`).
- I used [Learn OpenGL](https://learnopengl.com) to get up to speed on OpenGL. Also, some textures are taken from this awesome tutorial site.
- The book [3D Math Primer for Graphics and Game Development](https://gamemath.com) (along with various other sources) was used as the basis for math algorithms & classes.
- Some ideas, such as the (`Test` framework) are taken from [Cherno](https://github.com/TheCherno)'s awesome [OpenGL Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2) on YouTube.
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) from the [stb](https://github.com/nothings/stb) library was used for texture i/o.
- [Dear ImGui](https://github.com/ocornut/imgui) is used for the user interface.
- [GLFW](https://www.glfw.org/) is used for windowing & other platform-specific needs.
- [GLAD](https://github.com/Dav1dde/glad) is used as the loader for OpenGL functions.
