//Class header
#include "OpenGLView.hh"
//Local
#include "CocoaWindowBackend.hh"
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdpp;

@implementation OpenGLView
{
	_stdpp::CocoaWindowBackend *backend;
}

- (id)initWithBackend:(_stdpp::CocoaWindowBackend *)cocoaWindowBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = cocoaWindowBackend;
	return self;
}

//Override
- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];

	CocoaEventSource::EmitPaintEvent(self->backend->GetWidget());

	[[self openGLContext] flushBuffer];
}

@end