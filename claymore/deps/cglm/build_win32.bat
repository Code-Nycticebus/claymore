mkdir cglm-win
cd cglm-win
git clone https://github.com/recp/cglm.git

cmake -S ./cglm -B ./build -DCGLM_SHARED=OFF
msbuild ./build/cglm.sln /property:Configuration=Release
cd ..
