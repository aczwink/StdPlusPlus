set(SOURCE_FILES
        ${SOURCE_FILES}
        #.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

        #Containers
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/AFixedCharLengthString.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/C8BitString.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/CUTF32String.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/String.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/CConstUTF8StringIterator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/CUTF8String.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CUTF16String.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CConstUTF16StringIterator.cpp

        #Rendering
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CCubeMap.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CDeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CFrameBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CIndexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CInputState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CShader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CShaderProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CTexture2D.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CVertexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp

        #Streams
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/Readers/CTextReader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CBufferInputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileInputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CStdOut.cpp

		#Time
		${CMAKE_CURRENT_SOURCE_DIR}/Time/CCalendar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Time/CTime.cpp

        #UI
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidgetContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CMainWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CSizingPolicy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Layouts/CGridLayout.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/ATextRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDesktopRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/IRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/C3DView.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.cpp

        PARENT_SCOPE)