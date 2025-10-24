1. Install vcpkg [vcpkg](https://github.com/microsoft/vcpkg).
2. Install zlib (library) using vcpkg:
    bash: vcpkg install zlib

3. Install OpenSSL using vcpkg:
    bash: vcpkg install openssl

4. Integrate vcpkg with Visual Studio:
    bash: vcpkg integrate install

5. Open project / solution in Visual Studio .
6. Build solution.
7. Add builddirectory to you path.

You can now use the gitCode commands from everywhere.