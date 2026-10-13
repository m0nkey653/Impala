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

    class IAsset {
    public:
        virtual ~IAsset() = default;
        virtual std::string Name() = 0;
        virtual void Load() = 0;
    };

    class IAssetDataBase {
    public:
        virtual ~IAssetDataBase() = default;
        virtual void AddSpriteAtlas(const std::string& name, const std::string& sheetPath,
                                    const std::string& atlasPath) = 0;
    };

    typedef union GameEvent {

    } GameEvent;

    typedef struct GameContext {
        IGameRender2D* renderer2D;
        IGameRender3D* renderer3D;
        IAssetDataBase* assetDataBase;
    } GameContext;

    typedef struct GameInfo {
        std::string name;
        std::string version;
    } GameInfo;

    class IGame {
    public:
        virtual ~IGame() = default;
        virtual GameInfo GetGameInfo() const = 0;
        virtual void Init() = 0;
        virtual void Update(GameContext* context, float dt) = 0;
        virtual void Render(GameContext* context) = 0;
        virtual void OnEvent(const GameEvent& event) = 0;
        virtual void Shutdown() = 0;
    };

    IGame* CreateGame();
} // namespace impala