from conan import ConanFile

class MoxPPRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "PremakeDeps"

    def requirements(self):
        self.requires("fmt/11.1.3")             # Formatting
        self.requires("spdlog/1.15.1")          # Logging

    def configure(self):
        self.options["fmt"].with_unicode = False
        
        self.options["spdlog"].wchar_support = True
