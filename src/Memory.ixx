/*
 * Copyright (c) 2025-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.memory;

import vireo;
import lysa.types;

export namespace lysa {

    /**
     * Description of allocated GPU memory block
     */
    struct MemoryBlock {
        uint32 instanceIndex{0};
        size_t offset{0};
        size_t size{0};

        friend bool operator==(const MemoryBlock&first, const MemoryBlock&second) {
            return first.offset == second.offset && first.size == second.size;
        }
    };

    /**
     * Base class for all GPU memory arrays
     */
    class MemoryArray {
    public:
        /**
         * Allocate a new GPU memory block
         * @param instanceCount Number of resources instances stored in this memory block
         * @return
         */
        MemoryBlock alloc(size_t instanceCount);

        /**
         * Free a previously allocated GPU memory block
         * @param bloc Allocated memory block
         */
        void free(const MemoryBlock& bloc);

        /**
         * Schedule a data transfert from the CPU to the GPU.
         * Data will be temporarily written into a staging buffer.
         * @param destination Destination memory block
         * @param source Source address of CPU memory
         */
        virtual void write(const MemoryBlock& destination, const void* source) = 0;

        /**
         * Copy the entire content of this memory array to another GPU memory array
         * @param commandList Command list used for the copy operation
         * @param destination Destination GPU memory array
         */
        void copyTo(const vireo::CommandList& commandList, const MemoryArray& destination);

        /**
         * Returns the GPU memory buffer allocated for the entire array
         */
        auto getBuffer() const { return buffer; }

        virtual ~MemoryArray();
        MemoryArray(MemoryArray&) = delete;
        MemoryArray& operator=(MemoryArray&) = delete;

    protected:
        const std::string name;
        const size_t instanceSize;
        std::shared_ptr<vireo::Buffer> buffer;
        std::list<MemoryBlock> freeBlocs;
        std::mutex mutex;

        MemoryArray(
            const std::shared_ptr<vireo::Vireo>& vireo,
            size_t instanceSize,
            size_t instanceCount,
            vireo::BufferType bufferType,
            const std::string& name);
    };

    /**
     * Device-only GPU memory array
     */
    class DeviceMemoryArray : public MemoryArray {
    public:
        /**
         * Creates a device only GPU memory array
         * @param vireo Vireo instance
         * @param instanceSize Size in bytes of the resources stored in the array
         * @param instanceCount Maximum number of resources stored in the array
         * @param stagingInstanceCount Maximum number of temporary resources used for staging temporary data before transfer
         * @param name Name of the GPU buffer for GPU-side debug
         */
        DeviceMemoryArray(
            const std::shared_ptr<vireo::Vireo>& vireo,
            size_t instanceSize,
            size_t instanceCount,
            size_t stagingInstanceCount,
            vireo::BufferType,
            const std::string& name);

        void write(const MemoryBlock& destination, const void* source) override;

        /**
         * Transfer pending writes from the staging buffer into the array
         */
        void flush(const vireo::CommandList& commandList);

        /**
         * Put the GPU buffer in SHADER_READ state
         */
        void postBarrier(const vireo::CommandList& commandList) const;

        ~DeviceMemoryArray() override;

    private:
        std::shared_ptr<vireo::Buffer> stagingBuffer;
        size_t stagingBufferCurrentOffset{0};
        std::vector<vireo::BufferCopyRegion> pendingWrites;
    };

    /**
     * Host-accessible GPU memory array
     */
    class HostVisibleMemoryArray : public MemoryArray {
    public:
        /**
         * Creates a host-visible GPU memory array
         * @param vireo Vireo instance
         * @param instanceSize Size in bytes of resources stored in this array
         * @param instanceCount Maximum number of resources stores in this array
         * @param name Array name for GPU-side debug
         */
        HostVisibleMemoryArray(
            const std::shared_ptr<vireo::Vireo>& vireo,
            size_t instanceSize,
            size_t instanceCount,
            vireo::BufferType,
            const std::string& name);

        void write(const MemoryBlock& destination, const void* source) override;
    };

}
