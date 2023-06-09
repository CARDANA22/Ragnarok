# Set the compiler variables
CXX=g++ -std=c++17
CXXFLAGS+=-Wall -lX11 -lXi -lGL -lglfw3
LDFLAGS+=-L${CURDIR}/bin
OBJDIR=obj
OBJS=$(addprefix ${OBJDIR}/,$(subst .cpp,.o,${CXXFILES}))

# Get all the .h and .cpp files
HFILES:=$(shell find src/ -type f -iname *.h -print)
CXXFILES:=$(shell find src/ -type f -iname *.cpp -print)

# Make /bin/Ragnarok and let the user know of other commands
all: ${CURDIR}/bin/Ragnarok
	@echo "-----------------------------------------------"
	@echo "| Use \"make\" to build in Release mode         |"
	@echo "| Use \"make dev\" to build in Debug mode       |"
	@echo "| Use \"make clean\" to delete the object files |"
	@echo "-----------------------------------------------"

# Add more flags if being ran as dev mode
dev: CXXFLAGS+= -g -DDEBUG_CERR
dev: ${CURDIR}/bin/Ragnarok

# Remove all the object files in the obj/ folder
.PHONY: clean
clean:
	rm -r ${OBJDIR}

# Compile the .cpp files and output their object files to the OBJDIR
${CURDIR}/bin/Ragnarok: $(OBJS)
	@mkdir -p bin
	${CXX} -o ${CURDIR}/bin/Ragnarok $(OBJS) ${LDFLAGS} ${CXXFLAGS}

# Compile the binary from the object files
${OBJDIR}/%.o: %.cpp $(HFILES)
	@mkdir -p $$(dirname $@)
	$(CXX) -c -o $@ $< ${LDFLAGS} $(CXXFLAGS)
