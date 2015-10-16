FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
LIST (REMOVE_ITEM project_SRCS main*.cpp)

SET (project_LIB ${PROJECT_NAME})

ADD_LIBRARY (${project_LIB} ${project_SRCS})
SET_PROPERTY (TARGET ${project_LIB} PROPERTY CXX_STANDARD 11)
TARGET_LINK_LIBRARIES(${project_LIB} ${project_LIBS})
LIST (APPEND project_LIBS ${project_LIB})

SET (navigational_BIN "navigational_bin")
ADD_EXECUTABLE(${navigational_BIN} main_navigational.cpp)
TARGET_LINK_LIBRARIES(${navigational_BIN} ${project_LIBS})
SET_PROPERTY (TARGET ${navigational_BIN} PROPERTY CXX_STANDARD 11)

SET (datalog_BIN "datalog_bin")
ADD_EXECUTABLE(${datalog_BIN} main_datalog.cpp)
TARGET_LINK_LIBRARIES(${datalog_BIN} ${project_LIBS})
SET_PROPERTY (TARGET ${datalog_BIN} PROPERTY CXX_STANDARD 11)
