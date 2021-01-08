# Block Mapper

## Description
Block Mapper is a tool to create 3D levels out of small blocks models.

## Libraries Used
- [GLFW](https://www.glfw.org/)
- [GLAD](https://github.com/Dav1dde/glad)
- [assimp 4.1.0](https://www.assimp.org/)
- [tinyxml2](https://github.com/leethomason/tinyxml2)
- [Dear ImGui](https://github.com/ocornut/imgui)

## Compiling Instructions
Follow the instructions on [learnopengl.com](https://learnopengl.com/Getting-started/Creating-a-window) to install GLFW and GLAD. These libraries will need to be linked to the project. You will then need to install assimp 4.1.0 and also link it.

## Usage
Create the block models in a 3D modelling program (ex. [Blender](https://www.blender.org/)) and export the model as an .fbx file to the BlockEditor/Blocks folder. Color variants can be created by adding multiple UV maps to the model.

Use the merge group window to assign blocks to merge groups. The merge groups are combined into a single mesh when exported. Blocks not assigned to a merge group are automatically merged with adjacent blocks.

## BlockEditor Folder
A BlockEditor folder is created in your "My Documents" folder which contains subfolders for various files used by the application. The BlockEditor.zip files contains examples of the files in these folders. The subfolders are the following:

- **Blocks** contains the .fbx block files to place in the map.
- **Brushes** contains .xml files that define brush tools than can be used to place blocks in patterns (ex. in a checker pattern).
- **Export** contains the exported mesh.
- **Materials** contains the .xml material files.
- **Scenes** contains scenes saved by the application.
- **Textures** contains the textures used by the materials.

## Controls
- Camera: Scroll to zoom, click scroll wheel to rotate, Shift+click scroll wheel to pan
- Sets camera orbit point to select block: Delete
- Set camera mode to FPS controls: Shift+F
- Grid plane controls: +/-
- Rotate block brush: W/A/S/D/Q/E
- Delete selected block: X
- Undo: Ctrl+Z
- Save: Ctrl+S
- Open Merge Group Window: M

## Tools
- **Select Single** Selects a single block. Click empty space to deselect the block. Shift click to add a block to the selection.
- **Select Region** Selects all blocks attached to the clicked block.
- **Select Border** Selects all blocks on the outside of the clicked region.
- **Select Box** Drag select multiple blocks.
- **Select Plane** Selects all blocks attached to the selected block along a plane. This is meant for selecting walls/floors to add to a merge group.
- **Move** Move the selected blocks with WASD
- **Draw Single** Draws a single block.
- **Draw Line** N/A
- **Draw Rectangle** Click and drag to draw multiple blocks.
- **Draw Grid** N/A
- **Extrude** Use WASD to extrude the selected blocks. Uses the active brush for the new blocks. Click to place the blocks.
