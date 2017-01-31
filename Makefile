NVCC=nvcc
CFLAGS="-arch=sm_30"
LDFLAGS=-lcublas

PDWTCORE=src/wt.cu src/common.cu src/utils.cu src/separable.cu src/nonseparable.cu src/haar.cu src/filters.cpp


demo: src/demo.cpp src/io.cpp
	mkdir -p build
	$(NVCC) -g $(CFLAGS) -o build/demo $(PDWTCORE) src/demo.cpp src/io.cpp -lcublas

#~ demo:
#~ 	mkdir -p build
#~ 	$(NVCC) -g $(CFLAGS) -odir build  $(PDWTCORE) src/demo.cpp src/io.cpp
#~ 	$(NVCC) $(CFLAGS) -o build/demo build/*.o $(LDFLAGS)

testt: $(PDWTCORE) src/demo.cpp src/io.cpp
	$(NVCC) -g $(CFLAGS) -odir build -dc $^
	$(NVCC) $(CFLAGS) -o build/demo build/*.o $(LDFLAGS)
#~ 	$(NVCC) -g $(CFLAGS) -o testt $(PDWTCORE) src/demo.cpp src/io.cpp -lcublas





libpdwt.so:
	mkdir -p build
	$(NVCC) --ptxas-options=-v --compiler-options '-fPIC' -o build/$@ --shared $(PDWTCORE) $(CFLAGS) $(LDFLAGS)

# Double precision library
libpdwtd.so:
	mkdir -p build
	$(NVCC) --ptxas-options=-v --compiler-options '-fPIC' -o build/$@ --shared -DDOUBLEPRECISION $(PDWTCORE) $(CFLAGS) $(LDFLAGS)


#~ %.o: %.cu
#~ 	$(NVCC) -c $(CFLAGS) $<


clean:
	rm -rf build
