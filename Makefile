LDFLAGS=-losg -losgDB -losgViewer -lOpenThreads -lglut -lGL -lGLU	

PROG = animationController
HDRS = artPtRegistry.h keyFrame.h
SRCS = artPtRegistry.cc robotController.cc partControl.cc graphUtils.cc animationControl.cc keyFrame.cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

artPtrRegistry.o:artPtRegistry.cc artPtRegistry.h

robotController.o: robotController.cc artPtRegistry.h

partControl.o: partControl.cc artPtRegistry.h graphUtils.h

graphUtils.o: graphUtils.cc graphUtils.h

animationControl.o: animationControl.cc graphUtils.h artPtRegistry.h keyFrame.h

clean:
	-rm -f $(PROG) $(OBJS)

