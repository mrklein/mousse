include_directories(".")

set(CORE_SOURCES
	${CORE_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/dynamic_indexed_octree_name.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/dynamic_tree_data_point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/indexed_octree_name.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tree_data_cell.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/volume_type.cpp" PARENT_SCOPE)

