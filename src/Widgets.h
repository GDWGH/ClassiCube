#ifndef CC_WIDGETS_H
#define CC_WIDGETS_H
#include "Gui.h"
#include "BlockID.h"
#include "Constants.h"
#include "Entity.h"
/* Contains all 2D widget implementations.
   Copyright 2014-2019 ClassiCube | Licensed under BSD-3
*/
struct FontDesc;

/* A text label. */
struct TextWidget {
	Widget_Body
	struct Texture tex;
	PackedCol col;
};
#define TEXTWIDGET_MAX 4

/* Initialises a text widget. */
CC_NOINLINE void TextWidget_Make(struct TextWidget* w, 
								cc_uint8 horAnchor, cc_uint8 verAnchor, int xOffset, int yOffset);
/* Draws the given text into a texture, then updates the position and size of this widget. */
CC_NOINLINE void TextWidget_Set(struct TextWidget* w, const String* text, struct FontDesc* font);
/* Shorthand for TextWidget_Set using String_FromReadonly */
CC_NOINLINE void TextWidget_SetConst(struct TextWidget* w, const char* text, struct FontDesc* font);


typedef void (*Button_Get)(String* raw);
typedef void (*Button_Set)(const String* raw);
/* A labelled button that can be clicked on. */
struct ButtonWidget {
	Widget_Body
	struct Texture tex;
	int minWidth;
	const char* optName;
	Button_Get GetValue;
	Button_Set SetValue;
};
#define BUTTONWIDGET_MAX 12

/* Initialises a button widget. */
CC_NOINLINE void ButtonWidget_Make(struct ButtonWidget* w, int minWidth, Widget_LeftClick onClick, 
								cc_uint8 horAnchor, cc_uint8 verAnchor, int xOffset, int yOffset);
/* Draws the given text into a texture, then updates the position and size of this widget. */
CC_NOINLINE void ButtonWidget_Set(struct ButtonWidget* w, const String* text, struct FontDesc* font);
/* Shorthand for ButtonWidget_Set using String_FromReadonly */
CC_NOINLINE void ButtonWidget_SetConst(struct ButtonWidget* w, const char* text, struct FontDesc* font);

/* Clickable and draggable scrollbar. */
struct ScrollbarWidget {
	Widget_Body
	int totalRows, topRow;
	float scrollingAcc;
	int dragOffset;
	int draggingId;
};
/* Resets state of the given scrollbar widget to default. */
CC_NOINLINE void ScrollbarWidget_Create(struct ScrollbarWidget* w);

/* A row of blocks with a background. */
struct HotbarWidget {
	Widget_Body
	struct Texture selTex, backTex;
	float slotWidth, selWidth;
	float slotXOffset, elemSize;
	float scrollAcc;
	cc_bool altHandled;
};
/* Resets state of the given hotbar widget to default. */
CC_NOINLINE void HotbarWidget_Create(struct HotbarWidget* w);


/* A table of blocks. */
struct TableWidget {
	Widget_Body
	int blocksCount, blocksPerRow, rowsCount;
	int lastCreatedIndex;
	struct FontDesc* font;
	int selectedIndex, cellSize;
	float selBlockExpand;
	GfxResourceID vb;
	cc_bool pendingClose;

	BlockID blocks[BLOCK_COUNT];
	struct ScrollbarWidget scroll;
	struct Texture descTex;
	int lastX, lastY;
};

CC_NOINLINE void TableWidget_Create(struct TableWidget* w);
/* Sets the selected block in the table to the given block. */
/* Also adjusts scrollbar and moves cursor to be over the given block. */
CC_NOINLINE void TableWidget_SetBlockTo(struct TableWidget* w, BlockID block);
CC_NOINLINE void TableWidget_RecreateBlocks(struct TableWidget* w);
CC_NOINLINE void TableWidget_OnInventoryChanged(struct TableWidget* w);
CC_NOINLINE void TableWidget_MakeDescTex(struct TableWidget* w, BlockID block);
CC_NOINLINE void TableWidget_Recreate(struct TableWidget* w);


#define INPUTWIDGET_MAX_LINES 3
#define INPUTWIDGET_LEN STRING_SIZE
struct InputWidget {
	Widget_Body
	struct FontDesc* font;
	int  (*GetMaxLines)(void);
	void (*RemakeTexture)(void* elem);  /* Remakes the raw texture containing all the chat lines. Also updates dimensions. */
	void (*OnPressedEnter)(void* elem); /* Invoked when the user presses enter. */
	cc_bool (*AllowedChar)(void* elem, char c);
	void (*OnTextChanged)(void* elem); /* Callback invoked whenever text changes. */

	String text; /* The actual raw text */
	String lines[INPUTWIDGET_MAX_LINES];   /* text of each line after word wrapping */
	int lineWidths[INPUTWIDGET_MAX_LINES]; /* Width of each line in pixels */
	int lineHeight; /* Height of a line in pixels */
	struct Texture inputTex;
	int prefixWidth;
	cc_bool convertPercents;

	cc_uint8 padding;
	cc_bool showCaret;
	int caretWidth;
	int caretX, caretY; /* Coordinates of caret in lines */
	int caretPos;       /* Position of caret, -1 for at end of string */
	int caretOffset;
	PackedCol caretCol;
	struct Texture caretTex;
	double caretAccumulator;
};

/* Removes all characters and then deletes the input texture. */
CC_NOINLINE void InputWidget_Clear(struct InputWidget* w);
/* Tries appending all characters from the given string, then update the input texture. */
CC_NOINLINE void InputWidget_AppendText(struct InputWidget* w, const String* text);
/* Tries appending the given character, then updates the input texture. */
CC_NOINLINE void InputWidget_Append(struct InputWidget* w, char c);
/* Redraws text and recalculates associated state. */
/* Also calls Window_SetKeyboardText with the text in the input widget. */
/* This way native text input state stays synchronised with the input widget. */
/* (e.g. may only accept numerical input, so 'c' gets stripped from str) */
CC_NOINLINE void InputWidget_UpdateText(struct InputWidget* w);
/* Shorthand for InputWidget_Clear followed by InputWidget_AppendText, */
/* then calls Window_SetKeyboardText with the text in the input widget. */
/* This way native text input state stays synchronised with the input widget. */
/* (e.g. may only accept numerical input, so 'c' gets stripped from str) */
CC_NOINLINE void InputWidget_SetText(struct InputWidget* w, const String* str);


struct MenuInputDesc;
struct MenuInputVTABLE {
	/* Returns a description of the range of valid values (e.g. "0 - 100") */
	void (*GetRange)(struct MenuInputDesc*         d, String* range);
	/* Whether the given character is acceptable for this input */
	cc_bool (*IsValidChar)(struct MenuInputDesc*   d, char c);
	/* Whether the characters of the given string are acceptable for this input */
	/* e.g. for an integer, '-' is only valid for the first character */
	cc_bool (*IsValidString)(struct MenuInputDesc* d, const String* s);
	/* Whether the characters of the given string produce a valid value */
	cc_bool (*IsValidValue)(struct MenuInputDesc*  d, const String* s);
	/* Gets the default value for this input. */
	void (*GetDefault)(struct MenuInputDesc*       d, String* value);
};

struct MenuInputDesc {
	const struct MenuInputVTABLE* VTABLE;
	union {
		struct { const char* const* Names; int Count; } e;
		struct { int Min, Max, Default; } i;
		struct { float Min, Max, Default; } f;
		struct { PackedCol Default; } h;
	} meta;
};

extern const struct MenuInputVTABLE HexInput_VTABLE;
extern const struct MenuInputVTABLE IntInput_VTABLE;
extern const struct MenuInputVTABLE SeedInput_VTABLE;
extern const struct MenuInputVTABLE FloatInput_VTABLE;
extern const struct MenuInputVTABLE PathInput_VTABLE;
extern const struct MenuInputVTABLE StringInput_VTABLE;

#define MenuInput_Hex(v, def) v.VTABLE = &HexInput_VTABLE; v.meta.h.Default = def;
#define MenuInput_Int(v, lo, hi, def) v.VTABLE = &IntInput_VTABLE; v.meta.i.Min = lo; v.meta.i.Max = hi; v.meta.i.Default = def;
#define MenuInput_Seed(v) v.VTABLE = &SeedInput_VTABLE; v.meta.i.Min = Int32_MinValue; v.meta.i.Max = Int32_MaxValue;
#define MenuInput_Float(v, lo, hi, def) v.VTABLE = &FloatInput_VTABLE; v.meta.f.Min = lo; v.meta.f.Max = hi; v.meta.f.Default = def;
#define MenuInput_Path(v) v.VTABLE = &PathInput_VTABLE;
#define MenuInput_Enum(v, names, count) v.VTABLE = NULL; v.meta.e.Names = names; v.meta.e.Count = count;
#define MenuInput_String(v) v.VTABLE = &StringInput_VTABLE;

struct MenuInputWidget {
	struct InputWidget base;
	int minWidth, minHeight;
	struct MenuInputDesc desc;
	char _textBuffer[INPUTWIDGET_LEN];
};
#define MENUINPUTWIDGET_MAX 8

CC_NOINLINE void MenuInputWidget_Create(struct MenuInputWidget* w, int width, int height, const String* text, struct MenuInputDesc* d);
/* Sets the font used, then redraws the input widget. */
CC_NOINLINE void MenuInputWidget_SetFont(struct MenuInputWidget* w, struct FontDesc* font);


struct ChatInputWidget {
	struct InputWidget base;
	int typingLogPos;
	String origStr;
	char _textBuffer[INPUTWIDGET_MAX_LINES * INPUTWIDGET_LEN];
	char _origBuffer[INPUTWIDGET_MAX_LINES * INPUTWIDGET_LEN];	
};

CC_NOINLINE void ChatInputWidget_Create(struct ChatInputWidget* w);
CC_NOINLINE void ChatInputWidget_SetFont(struct ChatInputWidget* w, struct FontDesc* font);


/* Retrieves the text for the i'th line in the group */
typedef String (*TextGroupWidget_Get)(int i);
#define TEXTGROUPWIDGET_MAX_LINES 30
#define TEXTGROUPWIDGET_LEN (STRING_SIZE + (STRING_SIZE / 2))

/* A group of text labels. */
struct TextGroupWidget {
	Widget_Body
	int lines, defaultHeight;
	struct FontDesc* font;
	/* Whether a line has zero height when that line has no text in it. */
	cc_bool collapsible[TEXTGROUPWIDGET_MAX_LINES];
	cc_bool underlineUrls;
	struct Texture* textures;
	TextGroupWidget_Get GetLine;
};

CC_NOINLINE void TextGroupWidget_Create(struct TextGroupWidget* w, int lines, struct Texture* textures, TextGroupWidget_Get getLine);
CC_NOINLINE void TextGroupWidget_SetFont(struct TextGroupWidget* w, struct FontDesc* font);
/* Deletes first line, then moves all other lines upwards, then redraws last line. */
/* NOTE: GetLine must also adjust the lines it returns for this to behave properly. */
CC_NOINLINE void TextGroupWidget_ShiftUp(struct TextGroupWidget* w);
/* Deletes last line, then moves all other lines downwards, then redraws first line. */
/* NOTE: GetLine must also adjust the lines it returns for this to behave properly. */
CC_NOINLINE void TextGroupWidget_ShiftDown(struct TextGroupWidget* w);
/* Returns height of lines, except for the first 0 or more empty lines. */
CC_NOINLINE int  TextGroupWidget_UsedHeight(struct TextGroupWidget* w);
/* Returns either the URL or the line underneath the given coordinates. */
CC_NOINLINE void TextGroupWidget_GetSelected(struct TextGroupWidget* w, String* text, int mouseX, int mouseY);
/* Redraws the given line, updating the texture and Y position of other lines. */
CC_NOINLINE void TextGroupWidget_Redraw(struct TextGroupWidget* w, int index);
/* Calls TextGroupWidget_Redraw for all lines */
CC_NOINLINE void TextGroupWidget_RedrawAll(struct TextGroupWidget* w);
/* Calls TextGroupWidget_Redraw for all lines which have the given colour code. */
/* Typically only called in response to the ChatEvents.ColCodeChanged event. */
CC_NOINLINE void TextGroupWidget_RedrawAllWithCol(struct TextGroupWidget* w, char col);
/* Gets the text for the i'th line. */
static String TextGroupWidget_UNSAFE_Get(struct TextGroupWidget* w, int i) { return w->GetLine(i); }


typedef void (*SpecialInputAppendFunc)(void* userData, char c);
struct SpecialInputTab {
	int itemsPerRow, charsPerItem, titleWidth;
	String title, contents;	
};

struct SpecialInputWidget {
	Widget_Body
	int elementWidth, elementHeight;
	int selectedIndex;
	cc_bool pendingRedraw;
	struct InputWidget* target;
	struct Texture tex;
	struct FontDesc* font;
	int titleHeight;
	struct SpecialInputTab tabs[5];
	String colString;
	char _colBuffer[DRAWER2D_MAX_COLS * 4];
};

CC_NOINLINE void SpecialInputWidget_Create(struct SpecialInputWidget* w, struct FontDesc* font, struct InputWidget* target);
CC_NOINLINE void SpecialInputWidget_Redraw(struct SpecialInputWidget* w);
CC_NOINLINE void SpecialInputWidget_UpdateCols(struct SpecialInputWidget* w);
CC_NOINLINE void SpecialInputWidget_SetActive(struct SpecialInputWidget* w, cc_bool active);
#endif
