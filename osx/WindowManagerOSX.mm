//
//  WindowManagerOSX.cpp
//  TriumphX
//
//  Created by Justin Labak on 10/23/12.
//  Copyright (c) 2012 TwoBitInt. All rights reserved.
//

#include "WindowManager.h"
#import <Cocoa/Cocoa.h>

@interface MacDelegate : NSObject
{
}
@end

@implementation MacDelegate
- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}
@end

WindowManager::WindowManager()
{
    mNumWindows = 0;
}

Window* WindowManager::newWindow(int width, int height)
{
    if (mNumWindows < MAX_WINDOWS)
    {
        return new Window(width, height, mNumWindows++);
    }
    return NULL;
}

Window::Window(int width, int height, int id)
{
    mWidth = width;
    mHeight = height;
    mId = id;
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [NSApplication sharedApplication];
    
    NSRect frame = NSMakeRect(0, 0, width, height);
    
    unsigned int winStyle =
    NSTitledWindowMask | NSClosableWindowMask |
    NSMiniaturizableWindowMask | NSResizableWindowMask;
    
    NSWindow* window  = [[[NSWindow alloc] initWithContentRect:frame
                                                     styleMask:winStyle
                                                       backing:NSBackingStoreBuffered
                                                         defer:NO] autorelease];
    [window makeKeyAndOrderFront:NSApp];
    [window center];
    
	NSOpenGLPixelFormat *format;
	NSOpenGLPixelFormatAttribute formatAttrib[]=
	{
		NSOpenGLPFAWindow,
		NSOpenGLPFADepthSize,(NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFADoubleBuffer,
		0
	};
    
    // double buffer
    formatAttrib[3]=0;
    
	format=[NSOpenGLPixelFormat alloc];
	[format initWithAttributes: formatAttrib];
    
    frame = NSMakeRect(0, 0, width, height);
    NSOpenGLView *view = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat:format];
    [window setContentView: view];
    [window makeFirstResponder:view];
    
    NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
    [context setView:[window contentView]];
    [context makeCurrentContext];
    
    MacDelegate *appDelegate = [[MacDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
    [NSApp run];
    [pool release];
    
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    while (true)
    {
        [NSThread sleepForTimeInterval:0.01];
    }
    
}
