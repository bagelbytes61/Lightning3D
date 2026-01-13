cmake_minimum_required (VERSION 3.8)

project ("Lightning3D")

add_executable (${PROJECT_NAME} D3D12Renderer.cc Main.cc WindowsApp.cc WindowsWindow.cc  Games/Filler/Board.cc Games/Filler/Cell.cc Games/Filler/ColorSelectionPanel.cc Games/Filler/Filler.cc)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)

target_link_libraries(${PROJECT_NAME} PRIVATE d3d12.lib d3dcompiler.lib dxgi.lib)