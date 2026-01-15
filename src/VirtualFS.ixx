/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.virtual_fs;

import vireo;
import lysa.types;

export namespace lysa {

    /**
    * Configuration object used to initialize a Lysa instance.
    */
    struct VirtualFSConfiguration {
        //! Path for the app:// URI
        std::filesystem::path appDirectory{"."};
        //! Directory to search for Lua scripts inside app://
        std::string scriptsDir{"lib"};
        //! Directory to search for compiled shaders inside app://
        std::string shadersDir{"shaders"};
    };

    /**
     * Virtual file system helper used to resolve portable paths.
     */
    class VirtualFS {
    public:
        VirtualFS(const VirtualFSConfiguration& config, const std::shared_ptr<vireo::Vireo>& vireo) :
            vireo(vireo), config(config) {}

        /** URI scheme used to reference files relative to the application root. */
        static constexpr auto APP_URI{"app://"};

        /**
         * Tests whether a directory exists at the given path or URI.
         *
         * @param filepath URI.
         * @return True if the directory exists; false otherwise.
         */
        //static bool dirExists(Context& ctx, const std::string &filepath);

        /**
         * Tests whether a regular file exists at the given path or URI.
         *
         * @param filepath URI.
         * @return True if the file exists; false otherwise.
         */
        bool fileExists(const std::string &filepath) const;

        /**
         * Tests whether a directory exists at the given path or URI.
         *
         * @param dirPath URI.
         * @return True if the file exists; false otherwise.
         */
        bool directoryExists(const std::string& dirPath) const;

        /**
         * Opens an input stream for reading the file at path or URI.
         *
         * The returned stream is opened in binary mode unless the implementation
         * states otherwise. Callers should check stream.is_open() (or its state)
         * to ensure the file was successfully opened.
         *
         * @param filepath URI.
         * @return std::ifstream positioned at the start of the file.
         */
        std::ifstream openReadStream(const std::string &filepath) const;

        /**
         * Opens an output stream for writing the file at path or URI.
         *
         * The returned stream is typically opened in binary mode and may create
         * intermediate directories depending on platform support.
         * Callers should check stream.is_open() (or its state) to ensure the
         * file was successfully opened.
         *
         * @param filepath URI.
         * @return std::ofstream positioned at the start of the file.
         */
        std::ofstream openWriteStream(const std::string &filepath) const;

        /**
         * Returns the parent directory of the provided path or URI.
         *
         * For app:// URIs, the parent is computed after resolution.
         * Trailing separators are ignored.
         *
         * @param filepath URI.
         * @return Parent directory path, or an empty string if none.
         */
        std::string parentPath(const std::string& filepath) const;

        /**
         * Loads the entire file contents into a byte buffer.
         *
         * @param filepath URI.
         * @param out      Destination buffer; its contents are replaced by the file bytes.
         */
        void loadBinaryData(const std::string &filepath, std::vector<char>& out) const;

        /**
         * Loads the entire file contents into a byte buffer.
         *
         * @param input Input stream.
         * @param out   Destination buffer; its contents are replaced by the file bytes.
         */
        void loadBinaryData(std::ifstream& input, std::vector<char>& out) const;

        /**
         * Loads an image and returns an allocated RGBA (8‑bit per channel) buffer.
         *
         * The caller owns the returned memory and must release it with destroyImage().
         *
         * @param filepath URI.
         * @param width    Output image width in pixels.
         * @param height   Output image height in pixels.
         * @param size     Output total buffer size in bytes (width*height*4).
         * @return Pointer to the newly allocated pixel buffer in RGBA8888 format, or nullptr on failure.
         */
        std::byte* loadImage(const std::string& filepath, uint32& width, uint32& height, uint64& size) const;

        void loadScript(const std::string& scriptName, std::vector<char>& out) const;

        void loadShader(const std::string& shaderName, std::vector<char>& out) const;

        /**
         * Frees an image buffer allocated by loadImage().
         *
         * @param image Pointer previously returned by loadImage()
         */
        void destroyImage(std::byte* image) const;

        /**
         * Resolves a path or app:// URI to a concrete OS path.
         *
         * Implementations may expand environment variables, normalize separators,
         * and map app:// to the application data/assets directory.
         */
        std::string getPath(const std::string& filepath) const;

        constexpr const std::string& getScriptsDirectory() const { return config.scriptsDir; }

        constexpr std::string getScriptsURI() const { return APP_URI + config.scriptsDir; }

    private:
        const std::shared_ptr<vireo::Vireo> vireo;
        const VirtualFSConfiguration config;
    };

}
