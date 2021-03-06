//TODO: CLEAN UP CODE
//TODO: redisign menu options
//TODO: create a new Scalar class? Review inheritance
//TODO: at least overload some stuff to use add and div
//TODO: avg pixel stuff
//TODO: integrate with ffmpeg, remove use of System
//TODO: look at multithreading


#include <boost/thread.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <queue>

using namespace cv;
using namespace std;

Scalar add(Scalar frameMean, Scalar frameMeanTotal);
Scalar div(Scalar frameMeanTotal, double framesProccessed);
string avg(Vector<double> vec);
void progressBar(float progress);
String getResolution();

int main(int argc, char* argv[]) {

    // Variables to find avg time for operations
    Vector<double> readT;
    Vector<double> meanT;
    Vector<double> grabT;
    int frames = 0;

    // Variables for avg color
    Scalar frameMeanTotal;
    int framesProccessed = 0;

    // Variables for color colorCloud
    int cloudCols = 50;
    Mat colorCloud(1, cloudCols, CV_8UC3, Scalar(0,0,0));

    // get file from user
    // TODO: change this to getInfo()
    cout << "filename: ";
    string filename = "";
    cin >> filename;
    String resolution = getResolution();

    // see if they want to view timing stats
    cout << "View timing stats? (y/n) ";
    String tStats;
    cin >> tStats;

    // experimental:
    String compressCommand = "ffmpeg -i " + filename + " -loglevel quiet -s 640x360 -vcodec libx264 -ac 1 -b 64k -bt 64k -r " + resolution + " output.mp4";
    cout << "ffmpeg is compressing..." << endl;
    double tcompress = (double)getTickCount();
    system(compressCommand.c_str());
    tcompress = ((double)getTickCount() - tcompress)/getTickFrequency();

    VideoCapture cap("output.mp4"); // open the video file for reading

    // if not success, exit program
    if ( !cap.isOpened() ) {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    double taverage = (double)getTickCount();

    // Run until all frames have been read
    for (int i = 0; i < int(cap.get(CV_CAP_PROP_FRAME_COUNT)); i++) {

      // progress bar
      float progress = cap.get(CV_CAP_PROP_POS_FRAMES) / cap.get(CV_CAP_PROP_FRAME_COUNT);
      progressBar(progress);

      frames++;

      // look at but do not read the frame
      // get data to solve for avg t
      double t3 = (double)getTickCount();
      cap.grab();
      t3 = ((double)getTickCount() - t3) / getTickFrequency();
      grabT.push_back(t3);

      Mat frame;

      // retrieve frame and get data to solve for avg time to retrieve
      // get data to solve for avg t
      double t1 = (double)getTickCount();
      cap.retrieve(frame);
      t1 = ((double)getTickCount() - t1)/getTickFrequency();
      readT.push_back(t1);

      // Get avg color and data to calculate total avg color
      // get data to solve for avg time to calc mean
      double t2 = (double)getTickCount();
      Scalar frameMean = mean(frame);
      t2 = ((double)getTickCount() - t2)/getTickFrequency();
      meanT.push_back(t2);
      frameMeanTotal += frameMean;

      framesProccessed++;

      // adds a single row of the avg color to the colorCloud
      Mat row(1, cloudCols, CV_8UC3, frameMean);
      colorCloud.push_back(row);
    }

    cout << endl;

    // rotate and rescale colorCloud
    transpose(colorCloud, colorCloud);
    flip(colorCloud, colorCloud, 0); // now flipped by 90 CCW
    Mat temp = colorCloud.colRange(1, colorCloud.cols); // remove bank first col
    colorCloud = temp;
    Mat finalColorCloud(cloudCols, 1000, CV_8UC3);
    resize(colorCloud, finalColorCloud, finalColorCloud.size(), 0, 0, CV_INTER_LINEAR);

    // create avg color img
    Scalar avgColor = div(frameMeanTotal , double(framesProccessed));
    Mat img(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3, avgColor);

    if (img.empty()) {
      cout << "Error : Image cannot be loaded..!!" << endl;
      //system("pause"); //wait for a key press
      return -1;
     }

     // remove the output created by ffmpeg
     system("rm output.mp4");

     // Display avg color
     namedWindow("Total Average Color", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("Total Average Color", img); //display the image which is stored in the 'img' in the "MyWindow" window

     // Display color cloudloud
     namedWindow("Average Color Timeline", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("Average Color Timeline", finalColorCloud);

     // Timing stats
     // TODO: print from a function
     taverage = ((double)getTickCount() - taverage)/getTickFrequency();

     if (tStats[0] == 'y' || tStats[0] == 'Y') {
       cout << "Time to compress: " << tcompress << " s" << endl;
       cout << "Time to calculate average: " << taverage << " s" << endl;
       cout << "frames: " << frames << endl;
       cout << "frames analyzed: " << framesProccessed << endl;
       cout << "avg t for grab: " << avg(grabT) << endl;
       cout << "avg t for read: " << avg(readT) << endl;
       cout << "avg t for mean: " << avg(meanT) << endl;
     }
     cout << "\nTo save the output, press s when on the display window" << endl;


     char k = waitKey(0);  //wait infinite time for a keypress

     if (k == 's') {
       filename = filename.substr(0, filename.length()-5);
       String writeName = filename + "ColorCloud.jpg";
       imwrite(writeName, finalColorCloud);
       writeName = filename + "AverageColor.jpg";
       imwrite(writeName, img);
       destroyAllWindows();
     } else if (k == 'q') {
       destroyAllWindows(); //destroy all windows
     }

    return 0;

}

// used to add scalars
Scalar add(Scalar frameMean, Scalar frameMeanTotal) {

  for (int i = 0; i < 4; i++) {
    frameMeanTotal[i] += frameMean[i];
  }

  return frameMeanTotal;
}

// Used for dividing scalars
Scalar div(Scalar frameMeanTotal, double framesProccessed) {
  Scalar scal;

  for (int i = 0; i < 4; i++) {
    scal[i] = frameMeanTotal[i] / framesProccessed;
  }

  return scal;
}

// returns the average number from a vector for printing
string avg(Vector<double> vec) {
  double avg = 0;
  for (double i : vec) {
    avg += i;
  }
  return to_string(avg / vec.size());
}

// Displays a progress bar for the OpenCV section
void progressBar(float progress) {
  int barWidth = 70;
  cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; i++) {
    if (i < pos) std::cout << "=";
    else if (i == pos) std::cout << ">";
    else cout << " ";
  }
  cout << "] " << int(progress * 100.0) << " %\r";
  cout.flush();
}

String getResolution() {
  String x;
  cout << "Frames to analyze per second (may be less than 1): ";
  cin >> x;
  return x;
}
