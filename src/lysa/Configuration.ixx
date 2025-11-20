/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.configuration;

import std;
import vireo;
import lysa.enums;

export namespace lysa {

    struct ApplicationConfiguration {
        //! Directory to search for resources for the app:// URI
        std::filesystem::path  appDir{"."};
        //! Directory to search for compiled shaders inside app://
        std::string           shaderDir{"shaders"};
        //! Where to log a message using Logger
        int                    loggingMode{LOGGING_MODE_NONE};
        //! Minimum level for the log messages
        LogLevel               logLevelMin{LogLevel::INFO};
        //! Graphic API
        vireo::Backend         backend{vireo::Backend::VULKAN};

    };


}
