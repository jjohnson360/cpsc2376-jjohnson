# Gem 360 - Turn-Based Grid Game Design

## 1. Program Specification
A 2-player SDL2-based gem matching game where players take turns swapping adjacent gems to create matches. Key features:
- 8x8 grid of colored gems
- Turn-based gameplay with score tracking
- Animated gem swaps and cascading matches
- Win/lose conditions and restart capability
- Mouse-controlled interface with visual feedback
- Sound effects for game interactions

## 2. Core Entities and Interactions

**Key Entities:**
- `Game`: Manages game state and logic
- `Gem`: Individual grid elements with different colors
- `Player`: Score and turn management
- `Renderer`: Handles SDL2 graphics and animations

**Main Interactions:**
- Select and swap adjacent gems
- Detect and clear matches
- Animate gem movements
- Track scores and remaining moves
- Handle win/lose conditions
- Manage game state transitions

## 3. Class Architecture

### Game Class (Core Logic)
```cpp
class Game {
public:
    enum GemType { EMPTY, RED_GEM, GREEN_GEM, YELLOW_GEM, BLUE_GEM, MAGENTA_GEM };
    enum GameStatus { ONGOING, WIN, LOSE };
    enum Player { PLAYER_1, PLAYER_2 };

    // Grid configuration
    static const int GRID_SIZE = 8;
    static const int GEM_SIZE = 64;
    static const int GEM_SPACING = 2;
    
    Game();
    void reset();
    void update(float deltaTime);
    void draw(SDL_Renderer* renderer, ...textures);
    
    // State management
    bool isAnimating() const;
    GameStatus status() const;
    int getMovesLeft() const;
    
    // Player interaction
    void setSelectedGem(int row, int col);
    void play(int r1, int c1, int r2, int c2);
# Gem 360 - Turn-Based Grid Game Design

## 1. Program Specification
A 2-player SDL2-based gem matching game where players take turns swapping adjacent gems to create matches. Key features:
- 8x8 grid of colored gems
- Turn-based gameplay with score tracking
- Animated gem swaps and cascading matches
- Win/lose conditions and restart capability
- Mouse-controlled interface with visual feedback
- Sound effects for game interactions

## 2. Core Entities and Interactions

**Key Entities:**
- `Game`: Manages game state and logic
- `Gem`: Individual grid elements with different colors
- `Player`: Score and turn management
- `Renderer`: Handles SDL2 graphics and animations

**Main Interactions:**
- Select and swap adjacent gems
- Detect and clear matches
- Animate gem movements
- Track scores and remaining moves
- Handle win/lose conditions
- Manage game state transitions

## 3. Class Architecture

### Game Class (Core Logic)
```cpp
class Game {
public:
    enum GemType { EMPTY, RED_GEM, GREEN_GEM, YELLOW_GEM, BLUE_GEM, MAGENTA_GEM };
    enum GameStatus { ONGOING, WIN, LOSE };
    enum Player { PLAYER_1, PLAYER_2 };

    // Grid configuration
    static const int GRID_SIZE = 8;
    static const int GEM_SIZE = 64;
    static const int GEM_SPACING = 2;
    
    Game();
    void reset();
    void update(float deltaTime);
    void draw(SDL_Renderer* renderer, ...textures);
    
    // State management
    bool isAnimating() const;
    GameStatus status() const;
    int getMovesLeft() const;
    
    // Player interaction
    void setSelectedGem(int row, int col);
    void play(int r1, int c1, int r2, int c2);
};
```

## 4. Class Relationships Diagram (Mermaid)

```mermaid
classDiagram
    class Game {
        -board: GemType[8][8]
        -player1Score: int
        -player2Score: int
        -movesLeft: int
        -currentStatus: GameStatus
        +reset()
        +update(float deltaTime)
        +draw(SDL_Renderer*)
        +handleInput(SDL_Event)
    }
    
    class SDL_Renderer {
        +LoadTexture()
        +RenderCopy()
        +RenderPresent()
    }
    
    class GameState {
        <<enumeration>>
        START_SCREEN
        ONGOING
        GAME_OVER
    }
    
    class Player {
        <<enumeration>>
        PLAYER_1
        PLAYER_2
    }
    
    class GemType {
        <<enumeration>>
        EMPTY
        RED_GEM
        GREEN_GEM
        YELLOW_GEM
        BLUE_GEM
        MAGENTA_GEM
    }
    
    Game "1" --> "1" SDL_Renderer : Uses for rendering
    Game "1" *-- "1" GameState : Manages
    Game "1" o-- "64" GemType : Contains
    Game "1" --> "1" Player : Tracks current
    SDL_Renderer "1" *-- "many" SDL_Texture : Manages
    GameState "1" -- Game : Updates