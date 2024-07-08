from pathlib import Path

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy

class Recipe(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeToolchain', 'CMakeDeps'
    package_type = 'application'

    def requirements(self) -> None:
        self.requires('qt/6.7.1')

    def configure(self) -> None:
        self.options['qt'].shared = True

    def layout(self) -> None:
        cmake_layout(self)

    def generate(self) -> None:
        bin_folder = Path(self.build_folder)/str(self.settings.build_type)
        copy(self, '*.dll', Path(self.dependencies['qt'].package_folder)/'bin', bin_folder, overwrite_equal=True)
        copy(self, '*', Path(self.dependencies['qt'].package_folder)/'plugins', bin_folder, overwrite_equal=True, keep_path=True)
        