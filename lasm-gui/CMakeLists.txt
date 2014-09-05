CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(lasm-gui)
FIND_PACKAGE(Qt4 REQUIRED)


INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})


SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_INCLUDE_CURRENT_BINARY_DIR ON)
#SET(CMAKE_AUTOMOC TRUE)

SET(lasm-gui_SOURCES main.cpp qwmain.cpp)
SET(lasm-gui_HEADERS qwmain.h)

SET(lasm-gui_FORMS qwmain.ui)
SET(lasm-gui_RESOURCES lasm-gui.qrc)

QT4_WRAP_CPP(lasm-gui_HEADERS_MOC ${lasm-gui_HEADERS})
QT4_WRAP_UI(lasm-gui_FORMS_HEADERS ${lasm-gui_FORMS})
QT4_ADD_RESOURCES(lasm-gui_RESOURCES_RCC ${lasm-gui_RESOURCES})

ADD_EXECUTABLE(lasm-gui WIN32 ${lasm-gui_SOURCES} 
    ${lasm-gui_HEADERS_MOC} 
    ${lasm-gui_FORMS_HEADERS} 
    ${lasm-gui_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(lasm-gui ${QT_LIBRARIES})

IF (WIN32)
	TARGET_LINK_LIBRARIES(lasm-gui ${QT_QTMAIN_LIBRARY})
	
	STRING(REGEX REPLACE "[.]lib" ".dll" QT_DLL ${QT_LIBRARIES})
	STRING(REGEX REPLACE "lib" "bin" QT_DLL ${QT_DLL})
	 STRING(REGEX REPLACE "[.]dll" ".dll;" QT_DLL ${QT_DLL})

	FOREACH(DLL ${QT_DLL})
		if(${DLL} MATCHES "debug")
			STRING(REGEX REPLACE "^debug" "" QT_DLL_DBG ${DLL})
			list(APPEND QT_DLLS_DBG ${QT_DLL_DBG})
		elseif(${DLL} MATCHES "^optimized")
			STRING(REGEX REPLACE "^optimized" "" QT_DLL_REL ${DLL})
			list(APPEND QT_DLLS_REL ${QT_DLL_REL})
		ENDIF()

	ENDFOREACH(DLL)

	list(LENGTH QT_DLLS_REL len_var)


	if (MSVC)
	
	math(EXPR bound "${len_var} - 1")

	foreach(val RANGE ${bound})
	list(GET QT_DLLS_REL ${val} rel)
	list(GET QT_DLLS_DBG ${val} dbg)

	list(APPEND QT_DLL_SET "$<$<CONFIG:Debug>:${dbg}>$<$<CONFIG:Release>:${rel}>")

	endforeach()


	SET(DLL_LOC_DBG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	SET(DLL_LOC_REL ${CMAKE_CURRENT_BINARY_DIR}/Release)

	ENDIF()


	FOREACH (DLL ${QT_DLL_SET})

		ADD_CUSTOM_COMMAND(TARGET lasm-gui POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${DLL}" "$<$<CONFIG:Debug>:${DLL_LOC_DBG}>$<$<CONFIG:Release>:${DLL_LOC_REL}>")

	ENDFOREACH()

ENDIF(WIN32)

