:q!CFLAGS=-O2 -g `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`

all: GETBALL5

RCBOT: RCBot2.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o -I/usr/include/python3.7/ RCBot2.cpp -I/usr/local/include -L/usr/local/lib -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lwiringPi -lpthread -lpython3.7 -o RCBot2.exe

AUTOBOT: AUTOBot2.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o -I/usr/include/python3.7/ AUTOBot2.cpp -I/usr/local/include -L/usr/local/lib -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lwiringPi -lpthread -lpython3.7 -o AUTOBot.exe

GETBALL4: getball4.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision1.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision1.o -I/usr/include/python3.7/ getball4.cpp $(CFLAGS) $(LDFLAGS) -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 -lwiringPi -lpthread -o GETBALL4.exe

GETBALL5: getball5.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision2.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision2.o -I/usr/include/python3.7/ getball5.cpp $(CFLAGS) $(LDFLAGS) -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 -lwiringPi -lpthread -o GETBALL5.exe

GETBALL: getball2.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision1.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o vision1.o -I/usr/include/python3.7/ getball2.cpp $(CFLAGS) $(LDFLAGS) -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 -lwiringPi -lpthread -o GETBALL.exe

GETSPEED: GetSpeed.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o
	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o GetSpeed.cpp -I/usr/local/include -L/usr/local/lib -lwiringPi -lpthread -o GetSpeed.exe
	
#SERVOTEST: ServoTest.cpp GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o
#	g++ GPIOClassDef.o GPIOFunDef.o GOPIFunDef.o -I/usr/include/python3.7/ ServoTest.cpp -I/usr/local/include -L/usr/local/lib -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lwiringPi -lpthread -lpython3.7 -o ServoTest.exe

GPIOClassDef.o: GPIOClassDef.cpp
	g++ -c GPIOClassDef.cpp -lwiringPi
GPIOFunDef.o: GPIOFunDef.cpp
	g++ -c GPIOFunDef.cpp
GOPIFunDef.o: GOPIFunDef.cpp
	g++ -c -I/usr/include/python3.7/ GOPIFunDef.cpp -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7
vision1.o: vision1.cpp
	g++  -c vision2.cpp -I/usr/include/python3.7/ $(CFLAGS) $(LDFLAGS) -I/usr/local/include -L/usr/local/lib -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 -lwiringPi
vision2.o: vision2.cpp
	g++  -c vision2.cpp -I/usr/include/python3.7/ $(CFLAGS) $(LDFLAGS) -I/usr/local/include -L/usr/local/lib -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 -lwiringPi


test: test1.cpp
	g++ -I/usr/include/python3.7/ -o test.exe test1.cpp -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7 

servotest: ServoTest.cpp
	g++ -I/usr/include/python3.7/ -o servotest.exe ServoTest.cpp -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -lpython3.7
clean:
	rm *.o  *.exe

SimpleGrab: SimpleGrab.cpp
	g++  SimpleGrab.cpp -o SimpleGrab.exe $(CFLAGS) $(LDFLAGS)
GrabVideo: GrabVideo.cpp
	g++  GrabVideo.cpp -o GrabVideo.exe $(CFLAGS) $(LDFLAGS)

testdnn: testdnn2.cpp
	g++  testdnn2.cpp -o testdnn.exe -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_dnn -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
SimpleDnn: SimpleDnn.cpp
	g++  SimpleDnn.cpp -o simpleDnn.exe -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_dnn -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core

#testdnn: testdnn2.cpp
#	g++  testdnn2.cpp -o testdnn.exe $(CFLAGS) $(LDFLAGS) -I/usr/local/include/opencv4/ -L/usr/local/lib -lopencv_dnn -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
#-I/usr/local/include/opencv4/ -L/usr/local/lib -lopencv_dnn -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
