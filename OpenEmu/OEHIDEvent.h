/*
 Copyright (c) 2009, OpenEmu Team
 
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the OpenEmu Team nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Cocoa/Cocoa.h>
#import <IOKit/hid/IOHIDLib.h>

#define OEGlobalEventsKey @"OEGlobalEventsKey"

@class OEHIDDeviceHandler;

enum _OEHIDEventType {
    OEHIDAxis      = 1,
    OEHIDButton    = 2,
    OEHIDHatSwitch = 3,
	OEHIDKeypress  = 4,
};
typedef NSUInteger OEHIDEventType;

enum _OEHIDAxis {
    OEHIDAxisNone = 0x00,
    OEHIDAxisX    = 0x30,
    OEHIDAxisY    = 0x31,
    OEHIDAxisZ    = 0x32,
    OEHIDAxisRx   = 0x33,
    OEHIDAxisRy   = 0x34,
    OEHIDAxisRz   = 0x35
};
typedef NSUInteger OEHIDEventAxis;

enum _OEHIDDirection {
    OEHIDDirectionNegative = -1,
    OEHIDDirectionNull     =  0,
    OEHIDDirectionPositive =  1
};
typedef NSInteger OEHIDDirection;

@interface OEHIDEvent : NSObject <NSCoding>
{
@private
    OEHIDEventType         _type;
    NSUInteger             _padNumber;
    NSTimeInterval         _timestamp;
    NSTimeInterval         _previousTimestamp;
    NSUInteger             _cookie;
    union {
        struct {
            OEHIDEventAxis axis;
            OEHIDDirection previousDirection;
            OEHIDDirection direction;
            NSInteger      minimum;
            NSInteger      previousValue;
            NSInteger      value;
            NSInteger      maximum;
        } axis;
        struct {
            NSUInteger     buttonNumber;
            NSInteger      previousState;
            NSInteger      state;
        } button;
        struct {
            NSUInteger     count;
            NSUInteger     previousPosition;
            NSUInteger     position;
        } hatSwitch;
		struct {
			NSUInteger     keycode;
            NSInteger      previousState;
			NSInteger      state;
		} key;
    }                      _data;
    BOOL                   _hasPreviousState;
}

- (NSString *)displayDescription;

+ (NSUInteger)keyCodeForVK:(CGCharCode)charCode;
+ (id)axisEventWithPadNumber:(NSUInteger)padNumber timestamp:(NSTimeInterval)timestamp axis:(OEHIDEventAxis)axis scaledValue:(CGFloat)value;
+ (id)buttonEventWithPadNumber:(NSUInteger)padNumber timestamp:(NSTimeInterval)timestamp buttonNumber:(NSUInteger)number state:(NSUInteger)state cookie:(NSUInteger)cookie;
+ (id)hatSwitchEventWithPadNumber:(NSUInteger)padNumber timestamp:(NSTimeInterval)timestamp position:(NSUInteger)position positionCount:(NSUInteger)count cookie:(NSUInteger)cookie;
+ (id)keyEventWithTimestamp:(NSTimeInterval)timestamp keyCode:(NSUInteger)keyCode state:(NSUInteger)state cookie:(NSUInteger)cookie;
+ (id)eventWithDeviceHandler:(OEHIDDeviceHandler *)aDeviceHandler value:(IOHIDValueRef)aValue;

@property(readonly) NSUInteger     padNumber;
@property(readonly) NSTimeInterval timestamp;
@property(readonly) NSTimeInterval previousTimestamp;
@property(readonly) NSTimeInterval elapsedTime;
@property(readonly) OEHIDEventType type;
@property(readonly) NSUInteger     cookie;
@property(readonly) BOOL           hasPreviousState;

// Axis event
@property(readonly) OEHIDEventAxis axis;
@property(readonly) OEHIDDirection previousDirection;
@property(readonly) OEHIDDirection direction;
@property(readonly) NSInteger      minimum;
@property(readonly) NSInteger      previousValue;
@property(readonly) NSInteger      value;
@property(readonly) NSInteger      maximum;

// Button event
@property(readonly) NSUInteger     buttonNumber;
@property(readonly) NSInteger      previousState;
@property(readonly) NSInteger      state;

// Key event
@property(readonly) NSUInteger     keycode;
//@property(readonly) NSInteger      state;
//@property(readonly) NSInteger      previousState;

// HatSwitch event
@property(readonly) NSUInteger     count;
@property(readonly) NSUInteger     previousPosition;
@property(readonly) NSUInteger     position;
@end

@interface NSEvent (OEEventConversion)
+ (NSEvent *)eventWithKeyCode:(unsigned short)keyCode;
+ (NSEvent *)eventWithKeyCode:(unsigned short)keyCode keyIsDown:(BOOL)keyDown;
+ (NSString *)charactersForKeyCode:(unsigned short)keyCode;
+ (NSString *)printableCharactersForKeyCode:(unsigned short)keyCode;
+ (NSUInteger)modifierFlagsForKeyCode:(unsigned short)keyCode;
+ (NSString *)displayDescriptionForKeyCode:(unsigned short)keyCode;
- (NSString *)displayDescription;
@end

@interface NSNumber (OEEventConversion)
- (NSString *)displayDescription;
@end


