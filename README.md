Wizzard Engine (version 0.0.0)

The Wizzard Engine is intended to be a simple rendering application for me to explore and learn about working with different graphical APIs.

Eventually, it will also have basic level editing functionality, such as saving and loading scenes; importing art and audio assets; and placing and transforming (translation, rotation, and scaling) game objects.
The level editor will initially support only 2D applications, but my hope is to expand this to support 3D at some point in the future.

This application is only supported on x64 Windows, as I have no means of testing it on other platforms or architectures. It is being developed using Visual Studio 2022.

To Do (so far):
1. Ensure classes are abstract for the option to support multiple graphics APIs (i.e., have the option to switch between OpenGL and DirectX 11/12 easily).
2. Setup functionality for rendering windows with modern OpenGL and window events.
3. Integrate ImGui along with docking.
4. Profit!