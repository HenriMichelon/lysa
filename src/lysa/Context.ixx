/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.context;

import vireo;
import lysa.event;
import lysa.resources.locator;

export namespace  lysa {

    /**
     * @brief Lysa instance-wide runtime context.
     */
    struct Context {
        /**
         * @brief Quit flag controlling the main loop termination.
         *
         * When set to true, the main loop (see @ref Lysa::run) will exit
         * at the end of the current iteration.
         */
        bool exit{false};

        /**
         * @brief Central event dispatcher for the application.
         */
        EventManager eventManager;

        /**
         * @brief Resource resolution and access facility.
         */
        ResourcesLocator resourcesLocator;

        /**
         * @brief Backend object owning the device/instance and factory for GPU resources.
         */
        std::shared_ptr<vireo::Vireo> vireo;

        /**
         * @brief Submit queue used for graphics/rendering work.
         */
        std::shared_ptr<vireo::SubmitQueue> graphicQueue;

        //! Directory to search for resources for the app:// URI
        std::filesystem::path appDir;

        //! Directory to search for compiled shaders inside app://
        std::string shaderDir;
    };

}