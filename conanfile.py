from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
from conan.tools.env import VirtualBuildEnv
from conan.tools.files import load
import re, os

class mingtestConan(ConanFile):
    name = "mingtest"
    license = "MIT"
    author = "Colin Graf"
    url = "https://github.com/craflin/mingtest"
    description = "A minimalistic C++ unit test framework"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "include/*", "src/*", "CDeploy", "CMakeLists.txt", "LICENSE", "README.md"

    def set_version(self):
        content = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
        self.version = re.search("project\\([^ ]* VERSION ([0-9.]*)", content).group(1)

    def build_requirements(self):
        self.tool_requires("cmake/3.30.1")
        self.tool_requires("ninja/1.12.1")

    def generate(self):
        ms = VirtualBuildEnv(self)
        ms.generate()
        tc = CMakeToolchain(self, generator='Ninja')
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.builddirs.append(os.path.join("lib", "cmake", "mingtest"))
