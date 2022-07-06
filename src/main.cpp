#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>
#include <direct.h>
#include <Windows.h>
#include "func.h"
#include "ColorfulPrint.h"

#define MAX_PATH 260

using namespace std;
using namespace cv;

void SaveAllFrame(string file_path);
static void track_on_change(int, void*);
void mouse_on_change(int event, int x, int y, int flags, void* userdata);

VideoCapture video;
int current = 0;
struct VideoData
{
	string dir;
	string name;
	int width;
	int height;
}my_data;
Mat current_frame;
double resize_k = 1;

int main(int argc, char* argv[])
{
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	if (argc >= 3)
	{
		for (int i = 1; i < argc; i++)
		{
			SaveAllFrame(argv[i]);
		}
		printf("\n");
		system("pause");
	}
	else if (argc == 2)
	{
		string file_path = argv[1];
		string file_name = GetFileName(file_path);
		string file_dir = GetFileDirectory(file_path);

		my_data.dir = file_dir;
		my_data.name = file_name;
		
		video = VideoCapture(file_path);
		if (!video.isOpened())
		{
			SetFontColor(RED); printf("[FAILED]"); SetFontColor(WHITE);
			printf(" Can't Open Video File %s\n", file_path.c_str());
			system("pause");
		}
		int frame_count = video.get(CAP_PROP_FRAME_COUNT);
		my_data.width = video.get(CAP_PROP_FRAME_WIDTH);
		my_data.height = video.get(CAP_PROP_FRAME_HEIGHT);
		printf("Video: %s , Frames: %d\nDouble Click to Save Current Frame...\n", argv[1], frame_count);
		int temp = _mkdir((my_data.dir + "\\" + my_data.name).c_str());

		namedWindow(file_name);
		

		HWND hWnd = GetDesktopWindow();
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		// 获取监视器逻辑尺寸
		MONITORINFOEX miex;
		miex.cbSize = sizeof(miex);
		GetMonitorInfo(hMonitor, &miex);
		int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
		int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

		while (my_data.width > cxLogical|| my_data.height > cyLogical * 0.8)
		{
			resize_k *= 0.8;
			my_data.width *= resize_k; my_data.height *= resize_k;
		}
		resizeWindow(file_name, Size(my_data.width, my_data.height));

		setMouseCallback(file_name, mouse_on_change, 0);
		createTrackbar("Frame", file_name, &current, frame_count - 1, track_on_change, &my_data);
		
		track_on_change(current, 0);
		waitKey(0);
	}
	else
	{
		printf("请拖动要处理的视频到本程序上(用VideoFrame.exe打开)...\n");
		system("pause");
	}
	return 0;
}

static void track_on_change(int, void*)
{
	video.set(CAP_PROP_POS_FRAMES, current - 1);
	
	if (!video.read(current_frame))
	{
		return;
	}
	Mat temp;
	resize(current_frame, temp, Size(my_data.width, my_data.height));
	imshow(my_data.name, temp);
}

void mouse_on_change(int event, int x, int y, int flags, void* userdata)
{
	if (event == (EVENT_MOUSEMOVE | EVENT_LBUTTONDBLCLK))
	{
		char save_path[MAX_PATH];
		sprintf_s(save_path, "%s%s\\%d.png", my_data.dir.c_str(), my_data.name.c_str(), current);
		if (imwrite(save_path, current_frame))
		{
			SetFontColor(GREEN); printf("[SUCCEED]"); SetFontColor(WHITE);
			printf(" Saved an image: %s\n", save_path);
		}
		else
		{
			SetFontColor(RED); printf("[FAILED]"); SetFontColor(WHITE);
			printf(" Can't Save Image: %s\n", save_path);
		}
	}
}

void SaveAllFrame(string file_path)
{
	video = VideoCapture(file_path);
	if (!video.isOpened())
	{
		SetFontColor(RED); printf("[FAILED]"); SetFontColor(WHITE);
		printf(" Can't Open Video File %s\n", file_path.c_str());
		system("pause");
	}
	
	int frame_count = video.get(CAP_PROP_FRAME_COUNT);

	string file_name = GetFileName(file_path);
	string dir = GetFileDirectory(file_path) + file_name;
	int temp = _mkdir(dir.c_str());

	printf("\n开始处理 %s ...\n", file_name.c_str());
	for (int current = 0; current < frame_count; current++)
	{
		printf("\r已完成: %.1f %%", (static_cast<float>(current + 1) / frame_count) * 100);
		char file_name[MAX_PATH] = "";
		register cv::Mat img;
		video.read(img);
		if (!img.empty())
		{
			string save_name = dir + "\\%d.png";
			sprintf_s(file_name, save_name.c_str(), current);
			imwrite(file_name, img);
			memset(file_name, 0, sizeof(file_name));
		}
		else
		{
			break;
		}
	}
}
