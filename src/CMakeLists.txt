# 此为源码目录src所使用的CMakelists.txt

message("   -----------------------------------------------------------------")
message("--  +[目录src]")
message("--  +[链接库文件]: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/librouters.so")

add_subdirectory(routes)
add_subdirectory(app)
add_subdirectory(illuminate)

add_executable(${PROJECT_NAME} index.cc)

# 链接库文件，与link_libraries类似，但这里需要指定目标文件
target_link_libraries(${PROJECT_NAME} 
                      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/librouters.so
                      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libcontrollers.so
                      libhiredis.so
                      )
