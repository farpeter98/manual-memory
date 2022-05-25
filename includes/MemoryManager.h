#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <cstdint>
#include <cstdlib>
#include <list>

#include "ObjectBase.h"


namespace GarbageCollector {

class MemoryManager {
public:
    MemoryManager   ();
    ~MemoryManager  ();

    static MemoryManager&   GetInstance ();

    std::uint8_t**          Allocate    (std::size_t size);
    void                    Deallocate  (std::uint8_t** startAddressPtr);

    void                    PrintDescriptors    () const;
    void                    PrintMemory         () const;

private:
    void FreeAndCompact ();

    struct ObjectDescriptor {
        std::uint8_t*           startAddress;
        std::size_t             size;
        bool                    isLive;
    };

    static constexpr std::size_t    MEMORY_SIZE = 40;
    std::uint8_t*                   m_memory;
    std::list<ObjectDescriptor>     m_descriptors;
};

} // namespace GarbageCollector


#endif // MEMORYMANAGER_H