from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
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
        self.tool_requires("cmake/[>=3.27]")

    def generate(self):
        ms = VirtualBuildEnv(self)
        ms.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def layout(self):
        cmake_layout(self)

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "config")
        self.cpp_info.components["gtest"].set_property("cmake_target_name", "mingtest::gtest")
        self.cpp_info.components["gtest_main"].set_property("cmake_target_name", "mingtest::gtest_main")
        self.cpp_info.components["gtest_main"].requires = ["gtest"]
        if self.settings.get_safe("os") == "Windows" and self.settings.get_safe("build_type") == "Debug" and (self.settings.get_safe("compiler") == "Visual Studio" or self.settings.get_safe("compiler") == "msvc"):
            self.cpp_info.components["gtest"].libs = [ "gtestd" ]
            self.cpp_info.components["gtest_main"].libs = [ "gtest_maind" ]
        else:
            self.cpp_info.components["gtest"].libs = [ "gtest" ]
            self.cpp_info.components["gtest_main"].libs = [ "gtest_main" ]
