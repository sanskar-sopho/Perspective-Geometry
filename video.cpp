#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/calib3d/calib3d.hpp>
#include<iostream>
#define CUT 110

using namespace std;
using namespace cv;

vector<Point2f> in(4);
vector<Point2f> out(4);
Mat input;
void video();
void pts(int event,int x,int y,int flags,void*);
void modify(Mat*);

int main(int argc,char** argv)
{
	input=imread("checker_board.jpg",CV_LOAD_IMAGE_COLOR);
	namedWindow("image",WINDOW_NORMAL);
	imshow("image",input);
	setMouseCallback("image",pts);
	cout<<"Click 4 points and enter their coordinates";
	waitKey(0);
}

void pts(int event,int x,int y,int flags,void*)
{
	static int c=0;
	if(c>3)
		return;
	if(event==EVENT_LBUTTONDOWN)
	{
		in[c]=Point2f(x,y);	
		cout<<y<<' '<<x<<endl;
		cout<<"Enter x and y coordinates";
		int a,b;
		cin>>a>>b;
		out[c]=Point2f(a,b);
		c++;
		if(c==4)
			video();
	}
}

void video()
{
	Mat matrix=findHomography(in,out,CV_RANSAC);
	Mat matrix_inv=findHomography(out,in,CV_RANSAC);
	//cout<<"called";
	VideoCapture vc("road_video_2.webm");
	Mat frame,output;
	warpPerspective(input,output,matrix,output.size());
	namedWindow("output",WINDOW_NORMAL);
	modify(&output);
	imshow("output",output);
	cout<<output.size();
	waitKey(0);
	char ch;
	vc>>frame;
	//imshow("frame",frame);	
	//waitKey(0);
	while(frame.data && ch!=' ')
	{
		Mat output,output_inv;
		// imshow("video",frame);
		// waitKey(0);
		warpPerspective(frame,output,matrix,output.size());
		modify(&output);
		//imshow("video",output);
		warpPerspective(output,output_inv,matrix_inv,output.size());
		imshow("video",output_inv);
		ch==waitKey(10);
		vc>>frame;
	}
}

void modify(Mat* img)
{
	//cout<<"2";
	for(int i=0;i<CUT;i++)
		for(int j=0;j<img->rows;j++)
			img->at<Vec3b>(j,i)={0,0,0};
	for(int i=img->rows-CUT;i<img->cols;i++)
		for(int j=0;j<img->rows;j++)
			img->at<Vec3b>(j,i)={0,0,0};


	//cout<<"3";
	//imshow("output",*img);
}
