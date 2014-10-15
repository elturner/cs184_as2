CC = g++
CFLAGS = -g -O2 -W -Wall -Wextra -std=c++0x
LFLAGS = -lm #-lglut -lGL -lGLU
PFLAGS = #-pg -fprofile-arcs
IFLAGS = -Isrc/ -Iinclude/ 
BUILDDIR = build
EXECUTABLE = as2

# defines for the program

SOURCES =	include/lodepng/lodepng.cpp \
		src/util/cmd_args.cpp \
		src/io/raytrace_args.cpp \
		src/io/mesh/mesh_io.cpp \
		src/shape/aabb.cpp \
		src/geometry/transform.cpp \
		src/scene/phong_shader.cpp \
		src/scene/camera.cpp \
		src/scene/scene.cpp \
		src/gui/canvas.cpp \
		src/gui/sampler.cpp \
		src/main.cpp

HEADERS =	include/lodepng/lodepng.h \
		src/util/error_codes.h \
		src/util/cmd_args.h \
		src/io/raytrace_args.h \
		src/io/mesh/mesh_io.h \
		src/color/color.h \
		src/shape/shape.h \
		src/shape/aabb.h \
		src/shape/sphere.h \
		src/shape/triangle.h \
		src/shape/ray.h \
		src/geometry/transform.h \
		src/scene/light.h \
		src/scene/phong_shader.h \
		src/scene/element.h \
		src/scene/camera.h \
		src/scene/scene.h \
		src/gui/canvas.h \
		src/gui/sampler.h

OBJECTS = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# compile commands

all: $(SOURCES) $(EXECUTABLE)
	make --no-builtin-rules --no-builtin-variables $(EXECUTABLE)

simple:
	$(CC) $(IFLAGS) $(CFLAGS) $(LFLAGS) $(PFLAGS) $(SOURCES) -o $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS) $(PFLAGS) $(IFLAGS)

$(BUILDDIR)/%.o : %.cpp
	@mkdir -p $(shell dirname $@)		# ensure folder exists
	@g++ -std=c++0x -MM -MF $(patsubst %.o,%.d,$@) -MT $@ $< # recalc depends
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@

# helper commands

todo:
	grep -n --color=auto "TODO" $(SOURCES) $(HEADERS)

grep:
	grep -n --color=auto "$(SEARCH)" $(SOURCES) $(HEADERS)

size:
	wc $(SOURCES) $(HEADERS)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(BUILDDIR) $(EXECUTABLE).dSYM

# include full recalculated dependencies
-include $(OBJECTS:.o=.d)

