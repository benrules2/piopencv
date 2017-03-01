#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <omp.h>

using namespace std;
using namespace cv;


int main ( int argc,char **argv ) {

  raspicam::RaspiCam_Cv camera; 
  camera.set( CV_CAP_PROP_FORMAT, CV_8UC4 );
  camera.set( CV_CAP_PROP_MODE, 2);

  bool capture = false;
  cv::VideoWriter writer;
  
  if ( !camera.open()) {
      cerr<<"Error opening camera"<<endl;
      return -1;
  }

  Size sz = Size((int) camera.get(CV_CAP_PROP_FRAME_WIDTH), (int) camera.get(CV_CAP_PROP_FRAME_HEIGHT));

    if (argc > 1 && atoi(argv[1]) == 1)
      {
	printf("capture true \n");
	capture = true;
	writer.open("output.avi", CV_FOURCC('M','P','4','2'), 10, sz, true);
	if (!writer.isOpened())
	  printf("Error opening writer\n");
    }
    
  camera.grab();
  
  Mat frame;
  Mat frame_out;
  
  camera.retrieve ( frame);
  
  namedWindow("cam", CV_WINDOW_NORMAL);
  cvSetWindowProperty("cam", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
  
  bool terminate = false;
  
  
  //#pragma omp parallel num_threads(1) shared(terminate, frame, frame_out) firstprivate(camera)  default(none)
  
  while(!terminate)
    {
    
      camera.grab();
      camera.retrieve (frame);
      imshow("cam", frame);

      cvWaitKey(10);
      
      if (waitKey(10) == 27 || terminate)
	{
	  terminate = true;
	}

      if (capture)
	writer.write(frame);
    }

  camera.release();
  if (capture)
    writer.release();
  cout << "Returning cleanly" << endl;
  return 0;
  
}
