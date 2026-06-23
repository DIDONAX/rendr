# Rendr

A C++ library for low-overhead rendering when geometry and instance data change often. The focus is explicit GPU memory, batched draws, and streaming-friendly updates — not ray tracing or a full graphics engine.

CPU writes go directly into persistently mapped buffers. Meshes share a single vertex/index arena, instance data lives in SSBOs, and the frame is submitted with one `glMultiDrawElementsIndirect` call. Windowing and context setup stay on the application side; the bundled editor is just a demo for model loading.

Note: Vulkan is in progress since this project feels like a fight against the OpenGL api.

## Model Loading
![Teapot](docs/teapot.png)
![Robot](docs/robot.png)

## Dynamic Data Streaming
![Animation](docs/teapot.mp4)
