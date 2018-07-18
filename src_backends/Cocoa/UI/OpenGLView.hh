//Global
#include <Cocoa/Cocoa.h>

//Forward declarations
namespace _stdpp
{
	class CocoaWindowBackend;
}

@interface OpenGLView : NSOpenGLView
{
};
-(id)initWithBackend:(_stdpp::CocoaWindowBackend *)backend;
@end