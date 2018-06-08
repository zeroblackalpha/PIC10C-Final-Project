#!/bin/csh
setenv TBBROOT "C:\Users\ZeroB\Desktop\tbb-2018_U4"
setenv TBB_ARCH_PLATFORM "ia32\mingw6.3.0"
setenv TBB_TARGET_ARCH "ia32"
setenv CPATH "${TBBROOT}\include;$CPATH"
setenv LIBRARY_PATH "C:\Users\ZeroB\Desktop\tbb-2018_U4\build\windows_ia32_gcc_mingw6.3.0_debug;$LIBRARY_PATH"
setenv PATH "C:\Users\ZeroB\Desktop\tbb-2018_U4\build\windows_ia32_gcc_mingw6.3.0_debug;$PATH"
setenv LD_LIBRARY_PATH "C:\Users\ZeroB\Desktop\tbb-2018_U4\build\windows_ia32_gcc_mingw6.3.0_debug;$LD_LIBRARY_PATH"
