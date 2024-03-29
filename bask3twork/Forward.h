#pragma once
#include <cstdint>
#include <vector>



// Main
class App;
class MainWindow;
struct Version;



// wx
class wxBoxSizer;
class wxGridBagSizer;
class wxStaticText;
class wxStatusBar;
class wxString;
class wxTextCtrl;



// Controls
class ExportDialog;
class MenuBar;
class RegenDialog;
class RegenDialogTextBox;



// Grid
class DisplayGrid;
class Knot;

enum class TileLocked : bool;
enum class TileHighlighted : bool;
struct TileState;
class Tile;
using Tiles = std::vector<std::vector<Tile>>;



// Pure
enum class Connection : uint8_t;
struct ConnectionTransformations;
using ConnectionFn = Connection(*)(Connection);
struct Connections;

enum class GlyphFlag;
struct Glyph;
using Glyphs = std::vector<std::vector<const Glyph*>>;
struct GlyphsTransformed;
using CodePoint = int32_t;

struct GridSize;

struct Point;
struct Selection;

class SelectionIterator;
class SelectionZipIterator;
class SelectionZipRange;

enum class Symmetry;
class SymmetryChecker;



// Regions
class GenerateRegion;
class GenerateRegionButton;
class LockingRegion;
