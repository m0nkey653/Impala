namespace impala {

    class IGameRender {
    public:
        virtual ~IGameRender() = default;
    };

    typedef union ImpalaGameEvent {

    } ImpalaGameEvent;

    class Game {
    public:
        virtual ~Game() = default;
        virtual void Init() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render(IGameRender*) = 0;
        virtual void OnEvent(const ImpalaGameEvent&) = 0;
        virtual void Shutdown() = 0;
        virtual void run() = 0;
    };

    Game* CreateGame();
} // namespace impala