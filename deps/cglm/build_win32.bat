cmake -S ./cglm -B ./cglm/build -DCGLM_SHARED=OFF
msbuild ./cglm/build/cglm.sln /property:Configuration=Release /property:OutputPath=./cglm-win/