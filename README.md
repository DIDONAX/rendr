# Rendr

A C++ lightweight library for low-overhead rendering. The focus is explicit GPU memory handling, batched draws, and streaming-friendly updates — not ray tracing or a full graphics engine.

CPU writes go directly into persistently mapped buffers. Meshes share a single vertex/index arena, instance data lives in SSBOs, and the frame is submitted with one `glMultiDrawElementsIndirect` call. Windowing and context setup stay on the applicationside; the bundled editor is just for demo purposes.

Note: Vulkan rewrite is in progress since this project feels like a fight against the OpenGL api at this point.

## Model Loading
![Teapot](docs/teapot.png)
![Robot](docs/robot.png)

## Dynamic Data Streaming
![Animation](docs/teapot.mp4)
