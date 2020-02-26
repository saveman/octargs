set BASE_DIR=%CD%
set BUILD_DIR="%BASE_DIR%\_win_build
set INSTALL_DIR=%BASE_DIR%\_win_install
set PACKAGE_DIR=%BASE_DIR%\_win_package

rem ===== Do cleanup =====

rmdir /S /Q "%BUILD_DIR%"
rmdir /S /Q "%INSTALL_DIR%"
rmdir /S /Q "%PACKAGE_DIR%"

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
	.. ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Debug ^
	--target ALL_BUILD ^
	-- /m ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Debug ^
	--target RUN_TESTS ^
	-- /m ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target ALL_BUILD ^
	-- /m ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target INSTALL ^
	-- /m ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Release ^
	--target PACKAGE ^
	-- /m ^
	|| EXIT /B 1

echo "Success"

popd
