# CMake generated Testfile for 
# Source directory: /home/cos/Public/cstudy/cmake/tutorial
# Build directory: /home/cos/Public/cstudy/cmake/tutorial
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(TutorialRuns "Tutorial" "25")
ADD_TEST(TutorialComp25 "Tutorial" "25")
SET_TESTS_PROPERTIES(TutorialComp25 PROPERTIES  PASS_REGULAR_EXPRESSION "25 is 5")
ADD_TEST(TutorialNegative "Tutorial" "-25")
SET_TESTS_PROPERTIES(TutorialNegative PROPERTIES  PASS_REGULAR_EXPRESSION "-25 is 0")
ADD_TEST(TutorialSmall "Tutorial" "0.0001")
SET_TESTS_PROPERTIES(TutorialSmall PROPERTIES  PASS_REGULAR_EXPRESSION "0.0001 is 0.01")
ADD_TEST(TutorialUsage "Tutorial")
SET_TESTS_PROPERTIES(TutorialUsage PROPERTIES  PASS_REGULAR_EXPRESSION "Usage:.*number")
SUBDIRS(MathFunctions)
