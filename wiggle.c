/*
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Copyright (c) 2012 by Delphix. All rights reserved.
 */

#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>

static void
wiggle(void)
{
	CGEventRef now = CGEventCreate(NULL);
	CGPoint point = CGEventGetLocation(now);

	(void) printf("wiggle wiggle wiggle!\n");

	CGEventRef up = CGEventCreateMouseEvent(
	    NULL, kCGEventMouseMoved,
	    CGPointMake(point.x, point.y - 5),
	    kCGMouseButtonLeft);
	CGEventRef center = CGEventCreateMouseEvent(
	    NULL, kCGEventMouseMoved,
	    CGPointMake(point.x, point.y),
	    kCGMouseButtonLeft);
	CGEventRef down = CGEventCreateMouseEvent(
	    NULL, kCGEventMouseMoved,
	    CGPointMake(point.x, point.y + 5),
	    kCGMouseButtonLeft);
	int i;

	for (i = 0; i < 5; i++) {
		CGEventPost(kCGHIDEventTap, up);
		usleep(50000);
		CGEventPost(kCGHIDEventTap, down);
		usleep(50000);
	}
	CGEventPost(kCGHIDEventTap, center);

	// Release the events
	CFRelease(now);
	CFRelease(up);
	CFRelease(center);
	CFRelease(down);
}

static void
usage(void)
{
	(void) printf("Usage: wiggle [duration]\n");
	exit(2);
}

int
main(int argc, char **argv)
{
	int duration = 30;

	if (argc > 2)
		usage();

	if (argc == 2)
		duration = atoi(argv[1]);

	CGEventRef now = CGEventCreate(NULL);
	CGPoint point = CGEventGetLocation(now);
	CGFloat x, y;

	x = point.x;
	y = point.y;
	CFRelease(now);

	for (;;) {
		sleep(duration);
		now = CGEventCreate(NULL);
		point = CGEventGetLocation(now);

		if (fabs(x - point.x) + fabs(y - point.y) < 3)
			wiggle();

		x = point.x;
		y = point.y;
		CFRelease(now);
	}

	return 0;
}
