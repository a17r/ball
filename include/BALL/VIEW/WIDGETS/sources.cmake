### list all filenames of the directory here ###
SET(GROUP VIEW/WIDGETS)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

IF(NOT BALL_HAS_QTWEBENGINE)
	LIST(REMOVE_ITEM HEADERS_LIST
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/PresentaBALLView.h"
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/HTMLPage.h"
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/HTMLView.h"
	)
ENDIF()

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")
