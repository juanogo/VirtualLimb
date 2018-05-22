find_package(XAVIERSDK REQUIRED)

if(NOT XAVIERSDK_FOUND)
    message(ERROR " XAVIERSDK not found!")
endif(NOT XAVIERSDK_FOUND)

#Header files
set(HEADER_FILES myoController/myoController.h emotivController/emotivController.h CommunicationModule/Communication_WinSock.h)
#Source files
set(SOURCE_FILES myoController/myoController.cpp emotivController/emotivController.cpp CommunicationModule/Communication_WinSock.cpp ViLimbs_Console.cpp)

#Include directory
include_directories("../3rdparty/Myo/include" ${XAVIERSDK_INCLUDE_DIRS} ${XAVIERSDK_INCLUDE_DIR})
#Lib directory
link_directories("../../3rdparty/Myo/lib" ${XAVIERSDK_LINK_DIRS})

#Create .exe
add_executable(ViLimbs_Console ${SOURCE_FILES} ${HEADER_FILES})

#Name of .lib files to include in Input config
target_link_libraries(ViLimbs_Console ${XAVIERSDK_LIBRARIES} myo32 myo64 )

#Copy all files from specified directory to Output directory of the project
add_custom_command(TARGET ViLimbs_Console POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "../../3rdparty/Myo/dll"
        $<TARGET_FILE_DIR:ViLimbs_Console>)
		
add_custom_command(TARGET ViLimbs_Console POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "../../3rdparty/Emotiv/dll"
        $<TARGET_FILE_DIR:ViLimbs_Console>)

