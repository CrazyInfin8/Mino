#import <Cocoa/Cocoa.h>
#include <CoreGraphics/CoreGraphics.h>

#include "gamepad.h"
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "types.h"
#include "utils.h"
#include "window.h"

struct WindowNative {
    NSWindow *nsWindow;
};

@interface MinoWindowDelegate : NSObject <NSWindowDelegate> {
}
@end

@implementation MinoWindowDelegate

- (BOOL)windowShouldClose:(NSWindow *)sender {
    return true;
}

- (void)windowWillClose:(NSNotification *)notification {
    println("Window is about to close");
    [NSApp terminate:nil];
}

- (void)windowDidUpdate:(NSNotification *)notification {
    println("Window updated");
}
@end

@interface MinoAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}
@end

@implementation MinoAppDelegate
- (NSApplicationTerminateReply)applicationShouldTerminate:
    (NSApplication *)sender {
    println("Should terminate called");
    return NSTerminateNow;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    println("Application will launch");
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    window = [[NSWindow alloc]
        initWithContentRect:CGRectMake(0, 0, 640, 480)
                  styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                                NSWindowStyleMaskResizable)
                    backing:NSBackingStoreBuffered
                      defer:false];

    window.delegate = [MinoWindowDelegate alloc];

    // [window makeKeyAndOrderFront:nil];
    [window orderBack:nil];
    [window orderFront:nil];
    println("Window is front");
    [window center];
    println("Window is center");
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    [NSApp stop:nil];
}

- (void)applicationWillUpdate:(NSNotification *)notification {
    println("Application will update");
}

- (void)applicationDidUpdate:(NSNotification *)notification {
    println("Application updated");
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    println("Application will terminate");
}

@end

bool WindowInit(Window *window, WindowConfig config) {
    NSWindow *nsWindow = [[NSWindow alloc]
        initWithContentRect:CGRectMake(0, 0, config.width, config.height)
                  styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                                NSWindowStyleMaskResizable)
                    backing:NSBackingStoreBuffered
                      defer:false];

    nsWindow.delegate = [MinoWindowDelegate alloc];
    [nsWindow orderFront:nil];
    [nsWindow center];

    window->native = allocate(WindowNative);
    *window->native = (WindowNative){
        .nsWindow = nsWindow,
    }

    GamepadListInit(&window->gamepads, 0, 4);
}

bool WindowUpdate(Window *window) {
    for (;;) {
        NSEvent *evt = [NSApp nextEventMatchingMask:NSEventMaskAny
                                          untilDate:[NSDate distantPast]
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:true];

        if (evt == nil)
            break;
        switch (evt.type) {
            case NSEventTypeLeftMouseDown: {
                evt.buttonMask
                window->mousePressed = (uint8)setBit(window->mousePressed, MouseButton_Left);
            } break;

            case NSEventTypeRightMouseDown: {
                window->mousePressed = (uint8)setBit(window.mousePressed, MouseButton_Right)
            } break;
            case NSEventTypeOtherMouseDown: {
            } break;

            case NSEventTypeKeyDown:
            case NSEventTypeKeyUp: {
                println("Key code: %d", evt.keyCode);
            } break;

            default: {
            }
        }

        [NSApp sendEvent:evt];
        [evt release];
    }
}