//
//  Platform.h
//  Triumph
//
//  Created by Justin Labak on 10/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef Triumph_Platform_h
#define Triumph_Platform_h

#define PLATFORM_WIN32 0
#define PLATFORM_MAC32 1

#if defined(__WIN32__)
#define PLATFORM PLATFORM_WIN32
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC32
#endif

#endif
