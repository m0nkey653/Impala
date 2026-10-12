#include <string>

namespace impala {

    class IGameRender2D {
    public:
        virtual ~IGameRender2D() = default;
    };

    class IGameRender3D {
    public:
        virtual ~IGameRender3D() = default;
    };

    typedef union GameEvent {

    } GameEvent;

    typedef struct GameInfo {
        std::string name;
        std::string version;
    } GameInfo;

    class Game {
    public:
        virtual ~Game() = default;
        virtual GameInfo GetGameInfo() const = 0;
        virtual void Init() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render(IGameRender2D* renderer2D, IGameRender3D* renderer3D) = 0;
        virtual void OnEvent(const GameEvent& event) = 0;
        virtual void Shutdown() = 0;
    };

    Game* CreateGame();
} // namespace impala