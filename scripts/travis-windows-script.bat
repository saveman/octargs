set BASE_DIR=%CD%
set CPPUNIT_DIR=%BASE_DIR%\_win_cppunit
set CPPUNIT_INSTALL_DIR=%BASE_DIR%\_win_cppunit\install
set BUILD_DIR="%BASE_DIR%\_win_build
set INSTALL_DIR=%BASE_DIR%\_win_install
set PACKAGE_DIR=%BASE_DIR%\_win_package

rem ===== Do cleanup =====

rmdir /S /Q "%CPPUNIT_DIR%"
rmdir /S /Q "%BUILD_DIR%"
rmdir /S /Q "%INSTALL_DIR%"
rmdir /S /Q "%PACKAGE_DIR%"

rem ===== Build CPPUNIT =====

mkdir "%CPPUNIT_DIR%"
mkdir "%CPPUNIT_INSTALL_DIR%"

pushd "%CPPUNIT_DIR%"

curl --location --output cppunit.exe https://sourceforge.net/projects/cppunit-msvc/files/cppunit-msvc-1.12.1.exe

cppunit.exe /S /D=%CPPUNIT_INSTALL_DIR%

popd

rem ===== Build OCTARGS =====

mkdir "%BUILD_DIR%"

pushd "%BUILD_DIR%"

cmake ^
	-G "Visual Studio 15 2017 Win64" ^
	-DCMAKE_INSTALL_PREFIX:PATH="%INSTALL_DIR%" ^
	-DCPACK_PACKAGE_DIRECTORY:PATH="%PACKAGE_DIR%" ^
	-DBUILD_TESTS=True ^
	-DBUILD_EXAMPLES=True ^
	-DENABLE_COVERAGE=False ^
	-DCPPUNIT_ROOT_DIR:PATH="%CPPUNIT_INSTALL_DIR%" ^
	.. ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Debug ^
	--target ALL_BUILD ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Debug ^
	--target RUN_TESTS ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target ALL_BUILD ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target INSTALL ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target PACKAGE ^
	|| EXIT /B 1

echo "Success"

popd
