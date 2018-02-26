@ECHO OFF
cd .\runtime
FOR %%S in (fs_*.sc) DO (
 echo %%S
 ..\shaderc.exe -f %%S -o shaders\dx11\%%~nS.bin --platform windows --type fragment -O 3 -p ps_4_0 -i ..\external\bgfx\src
)
FOR %%S in (vs_*.sc) DO (
  echo %%S
  ..\shaderc.exe -f %%S -o shaders\dx11\%%~nS.bin --platform windows --type vertex  -O 3 -p vs_4_0 -i ..\external\bgfx\src
)
cd ..\