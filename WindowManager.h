#define MAX_WINDOWS 10

class Window;

class WindowManager
{
private:
	Window* mWindows[MAX_WINDOWS];
	int mNumWindows;

public:
	WindowManager();
	Window* newWindow(int width, int height);

};


class Window
{
private:
	int mId;
	WindowManager *mManager;

	int mWidth, mHeight;

public:
	Window(int width, int height, int id);

};