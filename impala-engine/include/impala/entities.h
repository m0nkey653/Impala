#pragma once

namespace impala {

    struct EntityHandle {
        uint64_t id;
        uint64_t count;
    };

    class IEntity {
    public:
        virtual ~IEntity() = default;
        virtual uint64_t Id() = 0;
        virtual std::string Name() = 0;
    };

} // namespace impala