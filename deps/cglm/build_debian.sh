mkdir cglm-linux
cd cglm-linux
git clone https://github.com/recp/cglm.git
cmake -S ./cglm -B ./build -DCGLM_SHARED=OFF # [Optional] 
make -C ./build
