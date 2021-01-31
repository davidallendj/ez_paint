from conans import ConanFile, CMake

class SpHash(ConanFile):
    name = "SpHash"
    version = "0.1.0"
    settings = ""
    generators = "cmake"

    def configure_cmake(self):
        pass

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

        # Run unit tests after the build
        cmake.test()

        # Run custom make command
        self.run("make -j4")

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()