include_directories(../../include)
link_libraries()
add_executable (TestSuite IteratorTestUtil.cpp Test_BasicDataStructures.cpp Test_BasicDataStructures_Tree.cpp Test_OuterBorderIterator.cpp Test_TriangleStripBorderIterator.cpp TestSuiteMain.cpp TestSuiteTest.cpp)

