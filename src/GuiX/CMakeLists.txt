include_directories(../../include)
link_libraries(X11 GL)
add_executable (GuiX GuiXMain.cpp ../GuiOpenGL/GuiComponentsBasic.cpp ../GuiOpenGL/GuiComponentsCheckBox.cpp ../GuiOpenGL/GuiComponentsTextBox.cpp ../GuiOpenGL/GuiOpenGLState.cpp)

