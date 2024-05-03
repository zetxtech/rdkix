cmake_minimum_required(VERSION 3.5)

project (jGraphMolJava Java)

include_directories( ${RDKix_ExternalDir} )

find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseJava)

# Make sure we can access the junit.jar -- needed for compiling/running tests
find_jar(JUNIT_JAR junit PATHS ${RDKix_JavaLibDir})
if (NOT JUNIT_JAR)
  if(NOT DEFINED JUNIT_URL)
    set(JUNIT_URL "https://repo1.maven.org/maven2/junit/junit/4.10/junit-4.10.jar")
  endif()
  if(NOT DEFINED JUNIT_MD5SUM)
    set(JUNIT_MD5SUM "68380001b88006ebe49be50cef5bb23a")
  endif()
  downloadAndCheckMD5(${JUNIT_URL} "${RDKix_JavaLibDir}/junit.jar" ${JUNIT_MD5SUM})
  find_jar(JUNIT_JAR junit PATHS ${RDKix_JavaLibDir})
  if (NOT JUNIT_JAR)
   	 MESSAGE ("junit.jar was not found in classpath or ${RDKix_JavaLibDir}.  Please add it to this directory and rerun cmake.")
	   MESSAGE(FATAL_ERROR "Cannot find required JUnit library (junit.jar)")
  endif()
endif (NOT JUNIT_JAR)

# Likewise for javadoc
find_program(JAVADOC_EXE javadoc PATH _JAVA_PATHS _JAVA_HINTS /usr/bin /usr/local/bin)
if (NOT JAVADOC_EXE)
	MESSAGE ("javadoc (executable) is not found. Please add it to PATH and rerun cmake.")
	MESSAGE(FATAL_ERROR "Cannot find required javadoc executable (javadoc)")
endif (NOT JAVADOC_EXE)

INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})

# java8 raises errors while creating the documentation
if(Java_VERSION_MAJOR GREATER 1 OR Java_VERSION_MINOR GREATER 7)
  set(X_DOCLINT_NONE "-Xdoclint:none")
  execute_process(COMMAND ${JAVADOC_EXE} ${X_DOCLINT_NONE}
                  OUTPUT_VARIABLE null
                  ERROR_VARIABLE err
                  ERROR_STRIP_TRAILING_WHITESPACE)
  if (NOT ${err} MATCHES "invalid flag")
    SET(DOCLINT_FLAGS ${X_DOCLINT_NONE})
  endif()
endif()

SET_SOURCE_FILES_PROPERTIES(GraphMolJava.i PROPERTIES CPLUSPLUS ON )

FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../*.java COPY_SOURCE)
FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/org/RDKix COPY_DEST)

# Setup a few variables for environment-specific things
if(MSVC)
  ADD_COMPILE_OPTIONS(/W3 /wd4716 /wd4101 /bigobj)
endif()
if(WIN32)
  SET(PATH_SEP ";")
  SET(COPY_CMD xcopy ${COPY_SOURCE} ${COPY_DEST} /Y /I)
else()
  SET(PATH_SEP ":")
  SET(COPY_CMD cp -p ${COPY_SOURCE} ${COPY_DEST})
endif()

# Coax SWIG into playing nicely with Apple environments
if(APPLE)
  SET(CMAKE_SIZEOF_VOID_P 4)
endif(APPLE)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  SET(CMAKE_SWIG_FLAGS -package "org.RDKix")
else()
  if (WIN32)
    SET(CMAKE_SWIG_FLAGS -package "org.RDKix")
  else()
      SET(CMAKE_SWIG_FLAGS -package "org.RDKix" "-DSWIGWORDSIZE64")
  endif()
endif()
SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/src/org/RDKix )

if(RDK_BUILD_INCHI_SUPPORT)
  SET(CMAKE_SWIG_FLAGS "-DRDK_BUILD_INCHI_SUPPORT" ${CMAKE_SWIG_FLAGS} )
endif()
if(RDK_BUILD_AVALON_SUPPORT)
  SET(CMAKE_SWIG_FLAGS "-DRDK_BUILD_AVALON_SUPPORT" ${CMAKE_SWIG_FLAGS} )
endif()
if(RDK_USE_BOOST_IOSTREAMS)
  SET(CMAKE_SWIG_FLAGS "-DRDK_USE_BOOST_IOSTREAMS" ${CMAKE_SWIG_FLAGS} )
endif()
if (RDK_BUILD_CAIRO_SUPPORT)
	SET(CMAKE_SWIG_FLAGS "-DRDK_BUILD_CAIRO_SUPPORT" ${CMAKE_SWIG_FLAGS} )
endif()


# enable this line to build the ErrorGenerator class for testing handling of C++ errors in the JNI layer
#SET(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-D INCLUDE_ERROR_GENERATOR" )

FILE(GLOB SWIG_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../*.i")

# we added all source files, now remove the ones that we're not supporting in this build:
if(NOT RDK_BUILD_AVALON_SUPPORT)
LIST(REMOVE_ITEM SWIG_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../AvalonLib.i")
endif()

if(NOT RDK_BUILD_INCHI_SUPPORT)
LIST(REMOVE_ITEM SWIG_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../Inchi.i")
endif()

SET(SWIG_MODULE_GraphMolWrap_EXTRA_DEPS ${SWIG_SRC_FILES} )

SWIG_ADD_LIBRARY(GraphMolWrap TYPE MODULE LANGUAGE java SOURCES GraphMolJava.i )

# it doesnt seem like the threading libs should need to be here, but
# as of Oct 2012 using boost 1.51 under at least ubuntu 12.04 we get a
# link error if they aren't there.
SWIG_LINK_LIBRARIES(GraphMolWrap ${RDKix_Wrapper_Libs}
      rdkix_base ${RDKix_THREAD_LIBS})

# code adapted from the wrapper code for
# GDCM: http://gdcm.svn.sf.net/viewvc/gdcm/trunk/Wrapping/Java/CMakeLists.txt?view=markup
ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar

  COMMAND ${CMAKE_COMMAND} -E make_directory build
  COMMAND ${CMAKE_COMMAND} -E make_directory build-test

  ## Add in our own Java sources
  # COMMAND ${COPY_CMD}
  ## 1. run this custom command only after swig has been run.
  COMMAND ${JAVA_COMPILE} -d build "src/org/RDKix/*.java"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  ## 2. now that the *.class have been generated construct the jar file.
  COMMAND ${JAVA_ARCHIVE} cf
    ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar -C ${CMAKE_CURRENT_SOURCE_DIR}/build
    org/RDKix
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  #DEPENDS GraphMolWrap
  DEPENDS "${swig_generated_file_fullname}"
  COMMENT "javac *.java; jar cf -> .jar"

  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKixDoc.jar
  ## Run Javadoc against newly created .java files to create docs
  COMMAND ${JAVADOC_EXE} ${DOCLINT_FLAGS} -tag notes -tag example -d ${CMAKE_CURRENT_SOURCE_DIR}/doc -sourcepath ${CMAKE_CURRENT_SOURCE_DIR}/src org.RDKix
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  ## Put the doc files into their own separate archive.
  COMMAND ${JAVA_ARCHIVE} cf
    ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKixDoc.jar
    doc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  #DEPENDS GraphMolWrap
  DEPENDS "${swig_generated_file_fullname}"
  COMMENT "jar cf org.RDKixDoc.jar -d doc"
)
ADD_CUSTOM_TARGET(GraphMolWrapJar ALL
  DEPENDS GraphMolWrap ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar
  COMMENT "building jar"
)

## Tests -- note that building/rebuilding them keys on the file WrapperTests.class

SET(CMAKE_JAVA_TEST_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/build-test )
FILE(GLOB JAVA_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src-test/org/RDKix/*.java")

# we added all source files, now remove the ones that we're not supporting in this build:
if(NOT RDK_BUILD_AVALON_SUPPORT)
LIST(REMOVE_ITEM JAVA_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src-test/org/RDKix/AvalonLibTests.java")
endif()

if(NOT RDK_BUILD_INCHI_SUPPORT)
LIST(REMOVE_ITEM JAVA_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src-test/org/RDKix/InchiTests.java")
endif()

if(NOT RDK_USE_BOOST_IOSTREAMS)
LIST(REMOVE_ITEM JAVA_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src-test/org/RDKix/GzStreamTests.java")
endif()

if (NOT RDK_BUILD_CAIRO_SUPPORT)
LIST(REMOVE_ITEM JAVA_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src-test/org/RDKix/MolDraw2DCairoTests.java")
endif()

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_JAVA_TEST_OUTDIR}/org/RDKix/WrapperTests.class
  COMMAND ${JAVA_COMPILE} ${DOCLINT_FLAGS} -d ${CMAKE_JAVA_TEST_OUTDIR} -cp "\"${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar${PATH_SEP}${JUNIT_JAR}\"" ${JAVA_TEST_FILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS GraphMolWrapJar ${CMAKE_CURRENT_SOURCE_DIR}/org.RDKixDoc.jar ${JAVA_TEST_FILES}
)
ADD_CUSTOM_TARGET(BuildJavaWrapperTests ALL
  DEPENDS ${CMAKE_JAVA_TEST_OUTDIR}/org/RDKix/WrapperTests.class GraphMolWrap
  COMMENT "building test classes"
)

# need to add boost libs for testing
SET (CTEST_ENVIRONMENT
  "LD_LIBRARY_PATH=${Boost_LIBRARY_DIRS}"
  "DYLD_LIBRARY_PATH=${Boost_LIBRARY_DIRS}"
  )

ADD_TEST(JavaAromaticTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.AromaticTests)
ADD_TEST(JavaAtomPairsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.AtomPairsTests)
ADD_TEST(JavaBasicMoleculeTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.BasicMoleculeTests)
ADD_TEST(JavaBasicMolecule2Tests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.BasicMolecule2Tests)
ADD_TEST(JavaChemAtomTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemAtomTests)
ADD_TEST(JavaChemBondTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemBondTests)
ADD_TEST(JavaChemReactionTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemReactionTests)
ADD_TEST(JavaChemSmartsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemSmartsTests)
ADD_TEST(JavaChemTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemTests)
ADD_TEST(JavaChemv2Tests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.Chemv2Tests)
ADD_TEST(JavaConformerTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ConformerTests)
ADD_TEST(JavaDescriptorTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.DescriptorTests)
ADD_TEST(JavaDistanceGeometryTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.DistanceGeometryTests)
ADD_TEST(JavaErrorHandlingTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ErrorHandlingTests)
ADD_TEST(JavaFingerprintsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.FingerprintsTests)
ADD_TEST(JavaForceFieldsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ForceFieldsTests)
ADD_TEST(JavaHManipulationsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.HManipulationsTests)
ADD_TEST(JavaLipinskiTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.LipinskiTests)
ADD_TEST(JavaPicklingTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.PicklingTests)
ADD_TEST(JavaSmilesCreationTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SmilesCreationTests)
ADD_TEST(JavaSmilesDetailsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SmilesDetailsTests)
ADD_TEST(JavaSmilesTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SmilesTests)
ADD_TEST(JavaSuppliersTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SuppliersTests)
if(RDK_USE_BOOST_IOSTREAMS)
ADD_TEST(JavaGzStreamTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.GzStreamTests)
endif(RDK_USE_BOOST_IOSTREAMS)
ADD_TEST(JavaTrajectoryTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.TrajectoryTests)
ADD_TEST(JavaWrapperTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.WrapperTests)

if(RDK_BUILD_AVALON_SUPPORT)
   ADD_TEST(JavaAvalonTests
      java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
  	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.AvalonLibTests)
endif(RDK_BUILD_AVALON_SUPPORT)

if (RDK_BUILD_INCHI_SUPPORT)
   ADD_TEST(JavaInchiTests
       java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
       	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
		org.RDKix.InchiTests)
endif (RDK_BUILD_INCHI_SUPPORT)

#ADD_TEST(JavaMemoryTests
#    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
#	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
#	org.RDKix.MemoryTests)

ADD_TEST(JavaChemTransformsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ChemTransformsTests)
ADD_TEST(JavaFMCSTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.FMCSTests)
ADD_TEST(JavaPDBTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.PDBTests)
ADD_TEST(JavaSequenceTests
      java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
  	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
  	org.RDKix.SequenceTests)

ADD_TEST(JavaAlignTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.AlignTests)

ADD_TEST(JavaMolQueryTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.MolQueryTests)

ADD_TEST(JavaFilterCatalogTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.FilterCatalogTests)

ADD_TEST(JavaSubstructLibraryTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SubstructLibraryTests)

ADD_TEST(JavaRGroupDecompositionTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.RGroupDecompositionTests)

ADD_TEST(JavaScaffoldNetworkTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.ScaffoldNetworkTests)

ADD_TEST(JavaTautomerQueryTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.TautomerQueryTests)

ADD_TEST(JavaMolEnumeratorTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.MolEnumeratorTests)

ADD_TEST(JavaMolHashTest
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.MolHashTest)

ADD_TEST(JavaAbbreviationsTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.AbbreviationsTests)

ADD_TEST(JavaDiversityPickerTests
	    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
		-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
		org.RDKix.DiversityPickerTests)

ADD_TEST(JavaBitOpsTests
	    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
		-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
		org.RDKix.BitOpsTests)

ADD_TEST(MolStandardizeTest
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
    -cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
    org.RDKix.MolStandardizeTest)

ADD_TEST(JavaSubstanceGroupTests
    java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
	-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
	org.RDKix.SubstanceGroupTests)

if (RDK_BUILD_CAIRO_SUPPORT)
	ADD_TEST(JavaMolDraw2DCairoTests
			java -Djava.library.path=${CMAKE_CURRENT_SOURCE_DIR}
			-cp "${JUNIT_JAR}${PATH_SEP}${CMAKE_JAVA_TEST_OUTDIR}${PATH_SEP}${CMAKE_CURRENT_SOURCE_DIR}/org.RDKix.jar"
			org.RDKix.MolDraw2DCairoTests)
endif (RDK_BUILD_CAIRO_SUPPORT)

INSTALL(TARGETS GraphMolWrap
              DESTINATION ${CMAKE_CURRENT_SOURCE_DIR} )
