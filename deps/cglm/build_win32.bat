cmake -S ./cglm -B ./cglm-win -DCGLM_SHARED=OFF
msbuild ./cglm-win/cglm.sln /property:Configuration=Release