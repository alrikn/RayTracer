##
## EPITECH PROJECT, 2026
## bootstrap_raytracer
## File description:
## Makefile
##

##
## EPITECH PROJECT, 2025
## nanotekspice
## File description:
## Makefile
##

NAME = raytracer
SRC = main.cpp \
    src/Positions/Vector3d.cpp \
    src/Positions/Point3d.cpp \
    src/RayTracer/shapes/Rectangle.cpp \
    src/RayTracer/shapes/Sphere.cpp \
    src/RayTracer/shapes/Plane.cpp \
    src/RayTracer/shapes/Cylinder.cpp \
    src/RayTracer/shapes/Cone.cpp \
    src/RayTracer/Camera.cpp \
    src/RayTracer/scene/Raytracing.cpp \
    src/RayTracer/scene/ThreadManagement.cpp \
    src/RayTracer/lights/DirectionalLight.cpp \
    src/RayTracer/lights/AmbientLight.cpp \
    src/RayTracer/lights/SpecularLight.cpp \
    src/Parser/Parser.cpp \
    src/Parser/LightCreator.cpp \
    src/Parser/ShapeCreator.cpp \
    src/Parser/AACreator.cpp \
    src/Parser/helper.cpp \
    src/AntiAliasing/Supersampling.cpp

OBJ = $(SRC:.cpp=.o)
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -O2 -fno-omit-frame-pointer
CPPFLAGS = -I include -I include/Shape -I include/Light -I include/Math

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ) -lconfig++

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

## ── Tests ──────────────────────────────────────────────────────────────────
## Framework: doctest v2.4.11 (single header in tests/doctest.h)
##
## make test             → build + run all three test tiers
## make test_unit        → unit tests only (fast, no I/O)
## make test_integration → Scene::traceRay() end-to-end tests
## make test_functional  → full 10×10 renders, pixel-level checks
##
## HOW IT WORKS:
##   LIB_OBJS = all project .o files except main.o
##   Each test tier compiles its own runner.cpp (defines main()) together
##   with all test_*.cpp files in that directory, then links LIB_OBJS.
##   No project .o file is recompiled — make reuses them from `all`.
## ────────────────────────────────────────────────────────────────────────────

TEST_DIR  = tests
TEST_CXX  = clang++
TEST_FLAGS = -std=c++20 -Wall -Wextra -g \
             -I include -I include/Shape -I include/Light -I include/Math \
             -I $(TEST_DIR)

# All compiled project objects except main.o (Parser excluded — needs libconfig++)
LIB_OBJS = src/Positions/Vector3d.o \
           src/Positions/Point3d.o \
           src/RayTracer/Camera.o \
           src/RayTracer/shapes/Sphere.o \
           src/RayTracer/shapes/Plane.o \
           src/RayTracer/shapes/Cylinder.o \
           src/RayTracer/shapes/Cone.o \
           src/RayTracer/shapes/Rectangle.o \
           src/RayTracer/lights/AmbientLight.o \
           src/RayTracer/lights/DirectionalLight.o \
           src/RayTracer/lights/SpecularLight.o \
           src/RayTracer/scene/Raytracing.o \
           src/RayTracer/scene/ThreadManagement.o \
           src/AntiAliasing/Supersampling.o

UNIT_SRCS        = $(wildcard $(TEST_DIR)/unit/*.cpp)
INTEGRATION_SRCS = $(wildcard $(TEST_DIR)/integration/*.cpp)
FUNCTIONAL_SRCS  = $(wildcard $(TEST_DIR)/functional/*.cpp)

build_tests: $(LIB_OBJS)
	$(TEST_CXX) $(TEST_FLAGS) $(UNIT_SRCS) $(LIB_OBJS) -o tests/rt_unit_tests
	$(TEST_CXX) $(TEST_FLAGS) $(INTEGRATION_SRCS) $(LIB_OBJS) -o tests/rt_integration_tests
	$(TEST_CXX) $(TEST_FLAGS) $(FUNCTIONAL_SRCS) $(LIB_OBJS) -o tests/rt_functional_tests

test_unit: $(LIB_OBJS)
	$(TEST_CXX) $(TEST_FLAGS) $(UNIT_SRCS) $(LIB_OBJS) -o tests/rt_unit_tests
	tests/rt_unit_tests

test_integration: $(LIB_OBJS)
	$(TEST_CXX) $(TEST_FLAGS) $(INTEGRATION_SRCS) $(LIB_OBJS) -o tests/rt_integration_tests
	tests/rt_integration_tests

test_functional: $(LIB_OBJS)
	$(TEST_CXX) $(TEST_FLAGS) $(FUNCTIONAL_SRCS) $(LIB_OBJS) -o tests/rt_functional_tests
	tests/rt_functional_tests

test: $(LIB_OBJS)
	@result=0; \
	$(TEST_CXX) $(TEST_FLAGS) $(UNIT_SRCS) $(LIB_OBJS) -o tests/rt_unit_tests && \
		tests/rt_unit_tests || result=1; \
	$(TEST_CXX) $(TEST_FLAGS) $(INTEGRATION_SRCS) $(LIB_OBJS) -o tests/rt_integration_tests && \
		tests/rt_integration_tests || result=1; \
	$(TEST_CXX) $(TEST_FLAGS) $(FUNCTIONAL_SRCS) $(LIB_OBJS) -o tests/rt_functional_tests && \
		tests/rt_functional_tests || result=1; \
	echo ""; \
	echo "══════════════════════════════════════════════════════════════════════════════"; \
	if [ $$result -eq 0 ]; then echo "  ALL TESTS PASSED"; else echo "  SOME TESTS FAILED — see details above"; fi; \
	echo "══════════════════════════════════════════════════════════════════════════════"; \
	printf "  Unit        │ "; tests/rt_unit_tests 2>/dev/null | grep "test cases" | sed 's/\[doctest\] //' | awk '{total=$$3; passed=$$5; pct=(total>0)?int(passed*100/total):0; printf "%s | %d%%\n", $$0, pct}'; \
	printf "  Integration │ "; tests/rt_integration_tests 2>/dev/null | grep "test cases" | sed 's/\[doctest\] //' | awk '{total=$$3; passed=$$5; pct=(total>0)?int(passed*100/total):0; printf "%s | %d%%\n", $$0, pct}'; \
	printf "  Functional  │ "; tests/rt_functional_tests 2>/dev/null | grep "test cases" | sed 's/\[doctest\] //' | awk '{total=$$3; passed=$$5; pct=(total>0)?int(passed*100/total):0; printf "%s | %d%%\n", $$0, pct}'; \
	echo "══════════════════════════════════════════════════════════════════════════════"; \
	exit $$result

## ────────────────────────────────────────────────────────────────────────────

flame_check: re
	perf record -g --call-graph dwarf ./raytracer > output.ppm
	perf script > out.perf
	./graph_test/stackcollapse-perf.pl out.perf > out.folded
	./graph_test/flamegraph.pl --width 2000 out.folded > docs/flame.svg
	rm -f docs/output.png
	ffmpeg -i output.ppm docs/output.png -loglevel quiet
	rm -f out.perf out.folded perf.data perf.data.old

clean:
	rm -f $(OBJ) $(CONVERT_OBJ)

fclean: clean
	rm -f $(NAME) $(CONVERT_NAME)
	rm -f default.profraw default.profdata lcov.info
	rm -rf coverage_report
	rm -f tests/rt_unit_tests tests/rt_integration_tests tests/rt_functional_tests

## ── Code Coverage ───────────────────────────────────────────────────────────
coverage: fclean
	@echo "Building tests with coverage flags..."
	@$(MAKE) build_tests \
		CXXFLAGS="$(CXXFLAGS) -fprofile-instr-generate -fcoverage-mapping" \
		TEST_FLAGS="$(TEST_FLAGS) -fprofile-instr-generate -fcoverage-mapping"
	@echo "Running tests to collect separate coverage profiles..."
	@LLVM_PROFILE_FILE="unit.profraw" tests/rt_unit_tests || true
	@LLVM_PROFILE_FILE="integration.profraw" tests/rt_integration_tests || true
	@LLVM_PROFILE_FILE="functional.profraw" tests/rt_functional_tests || true
	@echo "Merging all profile data..."
	@llvm-profdata-18 merge -sparse unit.profraw integration.profraw functional.profraw -o default.profdata
	@echo "Generating unified HTML report..."
	@llvm-cov-18 show \
		./tests/rt_unit_tests \
		-object ./tests/rt_integration_tests \
		-object ./tests/rt_functional_tests \
		-instr-profile=default.profdata \
		-format=html \
		-output-dir=coverage_report \
		-ignore-filename-regex="tests/.*|include/.*"
	@echo "Generating unified lcov.info..."
	@llvm-cov-18 export \
		./tests/rt_unit_tests \
		-object ./tests/rt_integration_tests \
		-object ./tests/rt_functional_tests \
		-instr-profile=default.profdata \
		-format=lcov \
		-ignore-filename-regex="tests/.*|include/.*" > lcov.info
	@echo "══════════════════════════════════════════════════════════════════════════════"
	@echo "✅ Complete unified coverage report generated!"
	@echo "   Open: coverage_report/index.html in your browser."
	@echo "══════════════════════════════════════════════════════════════════════════════"

re: fclean all