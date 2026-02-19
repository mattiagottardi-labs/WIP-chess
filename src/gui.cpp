#include "raylib.h"
#include "GameState.h"
#include "Player.h"
#include "Piece.h"
#include <iostream>
#include <string>

const int SQUARE_SIZE = 160;  // Increased from 80
const int BOARD_SIZE = 8 * SQUARE_SIZE;
const int SCREEN_WIDTH = BOARD_SIZE + 300;  // More space for sidebar
const int SCREEN_HEIGHT = BOARD_SIZE + 100;

#define LIGHT_SQUARE  currentTheme.light
#define DARK_SQUARE   currentTheme.dark
#define HIGHLIGHT     currentTheme.highlight
#define SELECTED      currentTheme.selected
#define LEGAL_MOVE    currentTheme.legal
#define CHECK_COLOR   currentTheme.check

struct Theme {
    Color light;
    Color dark;
    Color highlight;
    Color selected;
    Color legal;
    Color check;
};

Theme Themes[5] = {
    // THEME 0 — Classic brown
    {
        {240, 217, 181, 255},   // light
        {181, 136, 99, 255},    // dark
        {186, 202, 68, 180},    // highlight
        {246, 246, 105, 200},   // selected
        {100, 100, 100, 150},   // legal
        {255, 0, 0, 100}        // check
    },

    // THEME 1 — Blue
    {
        {200, 220, 255, 255},
        {90, 140, 190, 255},
        {255, 255, 0, 150},
        {255, 255, 120, 200},
        {120, 120, 120, 150},
        {255, 0, 0, 120}
    },

    // THEME 2 — Green
    {
        {220, 255, 220, 255},
        {120, 170, 110, 255},
        {255, 255, 0, 150},
        {255, 255, 120, 200},
        {120, 120, 120, 150},
        {255, 0, 0, 120}
    },

    // THEME 3 — Gray
    {
        {250, 250, 250, 255},
        {150, 150, 150, 255},
        {255, 255, 0, 150},
        {255, 255, 120, 200},
        {120, 120, 120, 150},
        {255, 0, 0, 120}
    },

    // THEME 4 — Purple
    {
        {240, 220, 255, 255},
        {160, 110, 190, 255},
        {255, 255, 0, 150},
        {255, 255, 120, 200},
        {120, 120, 120, 150},
        {255, 0, 0, 120}
    }
};

int currentThemeIndex = 0;
Theme currentTheme = Themes[0];

// Convert board coordinates to screen pixels
Vector2 boardToScreen(std::pair<char, int> pos) {
    return {(float)((pos.first - 'a') * SQUARE_SIZE), 
            (float)((8 - pos.second) * SQUARE_SIZE)};
}

// Convert screen pixels to board coordinates
std::pair<char, int> screenToBoard(Vector2 mousePos) {
    char file = 'a' + (int)(mousePos.x / SQUARE_SIZE);
    int rank = 8 - (int)(mousePos.y / SQUARE_SIZE);
    return {file, rank};
}

// Get Unicode chess piece symbol
const char* getPieceSymbol(Piece* piece) {// White pieces
    switch (piece->type) {
        case Piece::PieceType::King:   return piece->color ? "♔" : "♚";
        case Piece::PieceType::Queen:  return piece->color ? "♕" : "♛";
        case Piece::PieceType::Rook:   return piece->color ? "♖" : "♜";
        case Piece::PieceType::Bishop: return piece->color ? "♗" : "♝";
        case Piece::PieceType::Knight: return piece->color ? "♘" : "♞";
        case Piece::PieceType::Pawn:   return piece->color ? "♙" : "♟";
    }
    return "";
}
const char* getPieceSymbolByType(Piece::PieceType type, bool isWhite) {
    switch (type) {
        case Piece::PieceType::King:   return isWhite ? "♔" : "♚";
        case Piece::PieceType::Queen:  return isWhite ? "♕" : "♛";
        case Piece::PieceType::Rook:   return isWhite ? "♖" : "♜";
        case Piece::PieceType::Bishop: return isWhite ? "♗" : "♝";
        case Piece::PieceType::Knight: return isWhite ? "♘" : "♞";
        case Piece::PieceType::Pawn:   return isWhite ? "♙" : "♟";
    }
    return "?";
}


void drawBoard() {
    // Draw squares
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Color color = ((rank + file) % 2 == 0) ? LIGHT_SQUARE : DARK_SQUARE;
            DrawRectangle(file * SQUARE_SIZE, rank * SQUARE_SIZE, 
                         SQUARE_SIZE, SQUARE_SIZE, color);
        }
    }
    
    // Draw file labels (a-h)
    for (char file = 'a'; file <= 'h'; file++) {
        DrawText(TextFormat("%c", file), 
                (file - 'a') * SQUARE_SIZE + SQUARE_SIZE/2 - 10,
                BOARD_SIZE + 15, 28, DARKGRAY);
    }
    
    // Draw rank labels (1-8)
    for (int rank = 1; rank <= 8; rank++) {
        DrawText(TextFormat("%d", rank),
                BOARD_SIZE + 20,
                (8 - rank) * SQUARE_SIZE + SQUARE_SIZE/2 - 14,
                28, DARKGRAY);
    }
}

void highlightSquare(std::pair<char, int> pos, Color color) {
    Vector2 screen = boardToScreen(pos);
    DrawRectangle((int)screen.x, (int)screen.y, SQUARE_SIZE, SQUARE_SIZE, color);
}

void highlightMoves(std::vector<std::pair<char, int>>& moves, GameState* gs) {
    for (auto& move : moves) {
        Vector2 screen = boardToScreen(move);
        
        // Check if square has enemy piece (capture)
        auto it = gs->board.find(move);
        if (it != gs->board.end()) {
            // Draw ring for capture
            DrawRing({screen.x + SQUARE_SIZE/2.0f, screen.y + SQUARE_SIZE/2.0f}, 
                    SQUARE_SIZE/2.0f - 10, SQUARE_SIZE/2.0f - 5, 0, 360, 32, LEGAL_MOVE);
        } else {
            // Draw dot for empty square
            DrawCircle((int)(screen.x + SQUARE_SIZE/2), (int)(screen.y + SQUARE_SIZE/2), 
                      15, LEGAL_MOVE);
        }
    }
}

void drawPiece(Piece* piece, Vector2 pos, Font& font) {
    const char* symbol = getPieceSymbol(piece);

    int fontSize = 90;
    Vector2 textSize = MeasureTextEx(font, symbol, fontSize, 0);

    Vector2 drawPos = {
        pos.x + SQUARE_SIZE/2 - textSize.x/2,
        pos.y + SQUARE_SIZE/2 - textSize.y/2
    };

    bool isWhite = piece->color;

    // White pieces: white fill + black outline
    // Black pieces: black fill + white outline
    Color fillColor    = isWhite ? WHITE : BLACK;
    Color outlineColor = isWhite ? BLACK : WHITE;

    // --- OUTLINE (8 directions for a smooth border) ---
    const int o = 2; // outline thickness
    DrawTextEx(font, symbol, {drawPos.x - o, drawPos.y},     fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x + o, drawPos.y},     fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x,     drawPos.y - o}, fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x,     drawPos.y + o}, fontSize, 0, outlineColor);

    DrawTextEx(font, symbol, {drawPos.x - o, drawPos.y - o}, fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x + o, drawPos.y - o}, fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x - o, drawPos.y + o}, fontSize, 0, outlineColor);
    DrawTextEx(font, symbol, {drawPos.x + o, drawPos.y + o}, fontSize, 0, outlineColor);

    // --- FILL (centered) ---
    DrawTextEx(font, symbol, drawPos, fontSize, 0, fillColor);
}
void drawPieces(GameState* gs, Font& font) {
    for (auto& [pos, piece] : gs->board) {
        Vector2 screenPos = boardToScreen(pos);
        drawPiece(piece.get(), screenPos, font);
    }
}
void drawSidebar(Player* currentPlayer, GameState* gs, int moveCount) {
    int sidebarX = BOARD_SIZE + 30;
    int y = 30;   // vertical cursor for consistent spacing

    // --- Title ---
    DrawText("CHESS", sidebarX, y, 40, BLACK);
    y += 55;
    DrawRectangle(sidebarX, y, 220, 3, DARKGRAY);
    y += 30;

    // --- Turn Indicator ---
    DrawText("Turn", sidebarX, y, 26, DARKGRAY);
    y += 40;

    const char* turnText = currentPlayer->color ? "White" : "Black";
    Color turnColor = currentPlayer->color ? RAYWHITE : BLACK;
    Color textColor = currentPlayer->color ? BLACK : RAYWHITE;

    DrawRectangle(sidebarX, y, 160, 55, turnColor);
    DrawRectangleLines(sidebarX, y, 160, 55, DARKGRAY);
    DrawText(turnText, sidebarX + 40, y + 15, 28, textColor);
    y += 80;

    // --- Check Indicator ---
    if (gs->isInCheck(currentPlayer->color)) {
        DrawRectangle(sidebarX, y, 200, 60, (Color){255, 120, 120, 255});
        DrawRectangleLines(sidebarX, y, 200, 60, DARKGRAY);
        DrawText("CHECK!", sidebarX + 45, y + 15, 32, DARKBROWN);
        y += 90;
    }

    // --- Move Counter ---
    DrawText(TextFormat("Move: %d", moveCount), sidebarX, y, 24, DARKGRAY);
    y += 60;

    // --- Controls ---
    DrawText("Controls", sidebarX, y, 26, DARKGRAY);
    y += 35;
    DrawText("- Click piece to select", sidebarX, y, 22, GRAY);
    y += 25;
    DrawText("- Click destination", sidebarX, y, 22, GRAY);
    y += 25;
    DrawText("- ESC to quit", sidebarX, y, 22, GRAY);
    y += 60;

    // --- Captured Pieces ---
    DrawText("Captured", sidebarX, y, 26, DARKGRAY);
    y += 35;

    int whiteCaptured = 30 - gs->whitePieces.size();
    int blackCaptured = 30 - gs->blackPieces.size();

    DrawText(TextFormat("White: %d", whiteCaptured), sidebarX, y, 22, GRAY);
    y += 25;
    DrawText(TextFormat("Black: %d", blackCaptured), sidebarX, y, 22, GRAY);
}

bool hasLegalMoves(Player* player) {
    for (auto* piece : (player->color ? player->ga->whitePieces : player->ga->blackPieces)) {
        if (player->getRealScope(piece).size() > 0) {
            return true;
        }
    }
    return false;
}
bool Button(Rectangle r, const char* text) {
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, r);
    bool pressed = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(r, hover ? LIGHTGRAY : GRAY);
    DrawText(text, r.x + 10, r.y + 10, 20, BLACK);

    return pressed;
}
// Promotion dialog
Piece::PieceType showPromotionDialog(bool isWhite, Font& font) {
    const int dialogWidth = 500;
    const int dialogHeight = 250;
    const int dialogX = SCREEN_WIDTH/2 - dialogWidth/2;
    const int dialogY = SCREEN_HEIGHT/2 - dialogHeight/2;
    
    Piece::PieceType choices[4] = {
        Piece::PieceType::Queen,
        Piece::PieceType::Rook,
        Piece::PieceType::Bishop,
        Piece::PieceType::Knight
    };
    
    int selectedIndex = 0;
    bool confirmed = false;
    
    while (!confirmed && !WindowShouldClose()) {
        // Handle input
        if (IsKeyPressed(KEY_LEFT)) selectedIndex = (selectedIndex - 1 + 4) % 4;
        if (IsKeyPressed(KEY_RIGHT)) selectedIndex = (selectedIndex + 1) % 4;
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) confirmed = true;
        
        // Mouse input
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < 4; i++) {
                int boxX = dialogX + 30 + i * 110;
                int boxY = dialogY + 100;
                if (mousePos.x >= boxX && mousePos.x <= boxX + 100 &&
                    mousePos.y >= boxY && mousePos.y <= boxY + 100) {
                    selectedIndex = i;
                    confirmed = true;
                    break;
                }
            }
        }
        
        // Drawing
        BeginDrawing();
        
        // Dim background
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, 180});
        
        // Dialog box
        DrawRectangle(dialogX, dialogY, dialogWidth, dialogHeight, {240, 240, 240, 255});
        DrawRectangleLines(dialogX, dialogY, dialogWidth, dialogHeight, BLACK);
        
        // Title
        DrawText("Promote Pawn To:", dialogX + 120, dialogY + 30, 28, BLACK);
        
        // Draw choices
        for (int i = 0; i < 4; i++) {
            int boxX = dialogX + 30 + i * 110;
            int boxY = dialogY + 100;
            
            Color boxColor = (i == selectedIndex) ? SELECTED : LIGHT_SQUARE;
            DrawRectangle(boxX, boxY, 100, 100, boxColor);
            DrawRectangleLines(boxX, boxY, 100, 100, BLACK);
            
            // Draw piece symbol
            const char* symbol = getPieceSymbolByType(choices[i], isWhite);
            Vector2 textSize = MeasureTextEx(font, symbol, 60, 0);
            Vector2 drawPos = {
                boxX + 50 - textSize.x/2,
                boxY + 50 - textSize.y/2
            };
            
            DrawTextEx(font, symbol, drawPos, 60, 0, 
                      isWhite ? RAYWHITE : BLACK);
            if (isWhite) {
                Vector2 shadowPos = {drawPos.x + 1, drawPos.y + 1};
                DrawTextEx(font, symbol, shadowPos, 60, 0, {200, 200, 200, 255});
            }
        }
        
        // Instructions
        DrawText("Click or press ENTER to select", dialogX + 80, dialogY + 215, 16, DARKGRAY);
        
        EndDrawing();
    }
    
    return choices[selectedIndex];
}

int main() {
    // Window setup
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Game");
    SetTargetFPS(60);
    
    // Load font with chess symbols

    int start = 0x2600;
    int end   = 0x26FF;
    int count = end - start + 1;

    int* codepoints = (int*)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) codepoints[i] = start + i;

    Font chessFont = LoadFontEx("resources/chess_merida_unicode.ttf", 128, codepoints, count);
    SetTextureFilter(chessFont.texture, TEXTURE_FILTER_BILINEAR);

    free(codepoints);


    // Initialize game
    GameState gameState;
    Player white(&gameState, true);
    Player black(&gameState, false);
    Player* currentPlayer = &white;
    gameState.init();
    // UI state
    Piece* selectedPiece = nullptr;
    std::pair<char, int> selectedPos = {'z', 0};
    std::vector<std::pair<char, int>> legalMoves;
    int moveCount = 1;
    bool gameOver = false;
    std::string gameOverMessage = "";
    
    // Main game loop
    while (!WindowShouldClose()) {
        
        float panelX = BOARD_SIZE + 40;
        float btnW   = 200;
        float btnH   = 50;

        // Anchor point near bottom of window
        float bottomStart = GetScreenHeight() - 200;

        // Button positions
        float btnY_Theme     = bottomStart;
        float btnY_Surrender = bottomStart + 60;
        float btnY_PlayAgain = bottomStart + 120;

        // CHANGE THEME
        if (Button({panelX, btnY_Theme, btnW, btnH}, "Change Theme")) {
            currentThemeIndex = (currentThemeIndex + 1) % 5;
            currentTheme = Themes[currentThemeIndex];
        }

        // SURRENDER
        if (Button({panelX, btnY_Surrender, btnW, btnH}, "Surrender")) {
            gameOver = true;
            gameOverMessage = currentPlayer->color ? "Black Wins!" : "White Wins!";
        }

        // PLAY AGAIN
        if (Button({panelX, btnY_PlayAgain, btnW, btnH}, "Play Again")) {
            gameState.resetBoard();
            gameOver = false;
            currentPlayer = &white;
            gameOverMessage = "";
        }
        // Input handling
        if (!gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            
            // Check if click is on the board
            if (mousePos.x < BOARD_SIZE && mousePos.y < BOARD_SIZE) {
                auto clickedPos = screenToBoard(mousePos);
                
                // Validate coordinates
                if (clickedPos.first >= 'a' && clickedPos.first <= 'h' &&
                    clickedPos.second >= 1 && clickedPos.second <= 8) {
                    
                    // If piece already selected, try to move
                    if (selectedPiece) {
                        bool isLegalMove = false;
                        for (auto& move : legalMoves) {
                            if (move == clickedPos) {
                                isLegalMove = true;
                                break;
                            }
                        }
                        
                        if (isLegalMove) {
                            // Check if this is a promotion move
                            bool isPromotion = (selectedPiece->type == Piece::PieceType::Pawn && 
                                              clickedPos.second == (currentPlayer->color ? 8 : 1));
                            
                            // Execute move
                            currentPlayer->movePiece(selectedPiece, clickedPos);
                            
                            // Handle promotion with dialog
                            if (isPromotion) {
                                Piece::PieceType promoteTo = showPromotionDialog(currentPlayer->color, chessFont);
                                
                                // Find the promoted pawn (it's at clickedPos now)
                                auto it = gameState.board.find(clickedPos);
                                if (it != gameState.board.end()) {
                                    currentPlayer->promote(it->second.get(), promoteTo);
                                }
                            }
                            
                            // Switch players
                            currentPlayer = (currentPlayer == &white) ? &black : &white;
                            if (currentPlayer == &white) moveCount++;
                            
                            // Check for game over
                            if (!hasLegalMoves(currentPlayer)) {
                                gameOver = true;
                                if (gameState.isInCheck(currentPlayer->color)) {
                                    gameOverMessage = currentPlayer->color ? "Black Wins!" : "White Wins!";
                                } else {
                                    gameOverMessage = "Stalemate!";
                                }
                            }
                            
                            selectedPiece = nullptr;
                            legalMoves.clear();
                        } else {
                            // Clicked elsewhere - deselect or select new piece
                            auto it = gameState.board.find(clickedPos);
                            if (it != gameState.board.end() && 
                                it->second->color == currentPlayer->color) {
                                // Select new piece
                                selectedPiece = it->second.get();
                                selectedPos = clickedPos;
                                legalMoves = currentPlayer->getRealScope(selectedPiece);
                            } else {
                                // Deselect
                                selectedPiece = nullptr;
                                legalMoves.clear();
                            }
                        }
                    } else {
                        // No piece selected - try to select one
                        auto it = gameState.board.find(clickedPos);
                        if (it != gameState.board.end() && 
                            it->second->color == currentPlayer->color) {
                            selectedPiece = it->second.get();
                            selectedPos = clickedPos;
                            legalMoves = currentPlayer->getRealScope(selectedPiece);
                        }
                    }
                }
            }
        }
        
        // Drawing
        BeginDrawing();
        ClearBackground({50, 50, 50, 255});
        
        // Draw board
        drawBoard();
        
        // Highlight king in check
        if (gameState.isInCheck(currentPlayer->color)) {
            auto kingPos = currentPlayer->color ? gameState.whiteKingPosition : gameState.blackKingPosition;
            highlightSquare(kingPos, CHECK_COLOR);
        }
        
        // Highlight selected square and legal moves
        if (selectedPiece) {
            highlightSquare(selectedPos, SELECTED);
            highlightMoves(legalMoves, &gameState);
        }
        
        // Draw pieces
        drawPieces(&gameState, chessFont);
        
        // Draw sidebar
        drawSidebar(currentPlayer, &gameState, moveCount);
        
        // Draw game over message
        if (gameOver) {
            DrawRectangle(BOARD_SIZE/2 - 180, BOARD_SIZE/2 - 80, 360, 160, {0, 0, 0, 230});
            DrawRectangleLines(BOARD_SIZE/2 - 180, BOARD_SIZE/2 - 80, 360, 160, GOLD);
            DrawText(gameOverMessage.c_str(), 
                    BOARD_SIZE/2 - MeasureText(gameOverMessage.c_str(), 36)/2, 
                    BOARD_SIZE/2 - 40, 36, GOLD);
            DrawText("Press ESC to exit", 
                    BOARD_SIZE/2 - MeasureText("Press ESC to exit", 20)/2, 
                    BOARD_SIZE/2 + 20, 20, LIGHTGRAY);
        }
        
        EndDrawing();
    }
    
    UnloadFont(chessFont);
    CloseWindow();
    return 0;
}
