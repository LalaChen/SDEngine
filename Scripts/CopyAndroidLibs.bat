echo "Copy necessary android library."
echo "%0"
echo "%~fd0"
echo "%~dp0"
set SDEngineRootDir=%~dp0\..
echo "SDEngineRootDir: %SDEngineRootDir% %~dp0"
rem ----------------------------------- AndroidUnitTest
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Debug\*.so %SDEngineRootDir%\UnitTest\Android\unittest_featuredeveloping\libs\Debug\armeabi-v7a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Release\*.so %SDEngineRootDir%\UnitTest\Android\unittest_featuredeveloping\libs\Release\armeabi-v7a

xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Debug\*.so %SDEngineRootDir%\UnitTest\Android\unittest_featuredeveloping\libs\Debug\arm64-v8a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Release\*.so %SDEngineRootDir%\UnitTest\Android\unittest_featuredeveloping\libs\Release\arm64-v8a

xcopy /i /y /s %SDEngineRootDir%\Bin\UnitTest\Assets\UnitTest_FeatureDeveloping\*.* %SDEngineRootDir%\UnitTest\Android\unittest_featuredeveloping\src\main\assets

rem ----------------------------------- AndroidUnitTest_GraphicsAPI
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Debug\*.so %SDEngineRootDir%\UnitTest_GraphicsAPI\Android\unittest_graphicsapi\libs\Debug\armeabi-v7a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Release\*.so %SDEngineRootDir%\UnitTest_GraphicsAPI\Android\unittest_graphicsapi\libs\Release\armeabi-v7a

xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Debug\*.so %SDEngineRootDir%\UnitTest_GraphicsAPI\Android\unittest_graphicsapi\libs\Debug\arm64-v8a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Release\*.so %SDEngineRootDir%\UnitTest_GraphicsAPI\Android\unittest_graphicsapi\libs\Release\arm64-v8a

xcopy /i /y /s %SDEngineRootDir%\Bin\UnitTest\Assets\UnitTest_VulkanAPI\*.* %SDEngineRootDir%\UnitTest_GraphicsAPI\Android\unittest_graphicsapi\src\main\assets