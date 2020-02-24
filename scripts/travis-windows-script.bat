set BASE_DIR=%CD%
set BUILD_DIR="%BASE_DIR%\_win_build"
set INSTALL_DIR="%BASE_DIR%\_win_install"
set PACKAGE_DIR="%BASE_DIR%\_win_package"

rmdir /S /Q "%BUILD_DIR%"
rmdir /S /Q "%INSTALL_DIR%"
rmdir /S /Q "%PACKAGE_DIR%"

mkdir "%BUILD_DIR%"

pushd "%BUILD_DIR%"

cmake ^
	-DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
	-DCPACK_PACKAGE_DIRECTORY="%PACKAGE_DIR%" ^
	-DBUILD_TESTS=False ^
	-DBUILD_EXAMPLES=True ^
	-DENABLE_COVERAGE=False ^
	.. ^
	|| EXIT /B 1

cmake ^
	--build . ^
	--config Debug ^
	--target ALL_BUILD ^
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
