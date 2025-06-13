# CMake generated Testfile for 
# Source directory: C:/Users/gsneg/project4/text-editor
# Build directory: C:/Users/gsneg/project4/text-editor/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(tests "C:/Users/gsneg/project4/text-editor/build/Debug/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/gsneg/project4/text-editor/CMakeLists.txt;39;add_test;C:/Users/gsneg/project4/text-editor/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(tests "C:/Users/gsneg/project4/text-editor/build/Release/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/gsneg/project4/text-editor/CMakeLists.txt;39;add_test;C:/Users/gsneg/project4/text-editor/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(tests "C:/Users/gsneg/project4/text-editor/build/MinSizeRel/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/gsneg/project4/text-editor/CMakeLists.txt;39;add_test;C:/Users/gsneg/project4/text-editor/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(tests "C:/Users/gsneg/project4/text-editor/build/RelWithDebInfo/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/gsneg/project4/text-editor/CMakeLists.txt;39;add_test;C:/Users/gsneg/project4/text-editor/CMakeLists.txt;0;")
else()
  add_test(tests NOT_AVAILABLE)
endif()
