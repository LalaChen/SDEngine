echo "Copy necessary android library."
echo "%0"
echo "%~fd0"
echo "%~dp0"
set SDEngineRootDir=%~dp0\..
echo "SDEngineRootDir: %SDEngineRootDir% %~dp0"

xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Debug\*.so %SDEngineRootDir%\UnitTest\Android\unittest_androidapp\libs\Debug\armeabi-v7a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM\Release\*.so %SDEngineRootDir%\UnitTest\Android\unittest_androidapp\libs\Release\armeabi-v7a

xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Debug\*.so %SDEngineRootDir%\UnitTest\Android\unittest_androidapp\libs\Debug\arm64-v8a
xcopy /i /y %SDEngineRootDir%\Bin\UnitTest\Android\ARM64\Release\*.so %SDEngineRootDir%\UnitTest\Android\unittest_androidapp\libs\Release\arm64-v8a