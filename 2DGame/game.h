// ==========================================================
//
// ゲーム[game.h]
// Author:Ryouma Inoue
//
// ==========================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define	BLOCK_FILE "data/Block.txt"			// 読み込むファイル名
#define	ITEM_FILE "data/Item.txt"			// 読み込むファイル名
#define BLOCKSIZE_X (100)
#define BLOCKSIZE_Y (100)
#define ITEMSIZE_X (60)
#define ITEMSIZE_Y (60)
#define BG_POS_X (640.0f)
#define BG_POS_Y (360.0f)
#define PLAYER_POS_X (1000.0f)
#define PLAYER_POS_Y (500.0f)
#define EGG_POS_X (2400.0f)
#define EGG_POS_Y (600.0f)
#define EFFECT_POS_X (2400.0f)
#define EFFECT_POS_Y (550.0f)
#define WALL_POS_X (1230.0f)
#define WALL_POS_Y (420.0f)
#define LIFE_POS_X (100)
#define LIFE_POS_Y (40)
#define LIFE_SIZE_X (500)
#define LIFE_SIZE_Y (35)
#define SCROLL_MOVE (0.003f)	// スクロールのスピード
#define SCROLL_POS (4.0f)
#define STATE_COUNT (90)
#define WALL_NULL (1)

class CPlayer;
class CNumber;
class CEgg;
class CLief;
class CWall;
class CRecord;
class CBlock;
class CItem;

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CGame
{
public:
	// ----------------------------------------------------------------------------
	// ゲームの状態
	// ----------------------------------------------------------------------------
	typedef enum
	{
		GAMESTATE_NONE = 0,
		GAMESTATE_TUTORIAL,
		GAMESTATE_TUTORIALCLOSE,
		GAMESTATE_STARTTELOP,
		GAMESTATE_NORMAL,
		GAMESTATE_PAUSE,
		GAMESTATE_END,
		GAMESTATE_CLEAR,
		GAMESTATE_STAGEEND,
		GAMESTATE_MAX
	} GAMESTATE;

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ

	void InitGame(void);
	void UninitGame(void);
	void UpdateGame(void);
	void DrawGame(void);

	GAMESTATE GetGameState(void);

	static void SetGameState(GAMESTATE state);
	static bool LoadBlock(int nStageNo);
	static bool LoadItem(int nStage);
	static CPlayer *GetPlayer(void);
	static CBlock *GetBlock(void);
	static CEgg *GetEgg(void);
	static CWall *GetWall(void);
	static CItem *GetItem(void);
	static CGame *Create(void);

protected:

private:
	int		  m_nCntStage;

	static int m_nCounterGameState;
	static GAMESTATE m_gameState;
	static int m_nCntMove;
	static float m_fpos;
	static float m_fposEgg;
	static D3DXVECTOR3 m_pos;		// 位置
	static D3DXVECTOR3 m_move;		// 移動
	static int m_nLife;
	static int m_nNumBlock;
	static int m_nNumItem;
	static CPlayer *m_pPlayer;
	static CBlock *m_pBlock;
	static CNumber *m_pNumber;
	static CEgg *m_pEgg;
	static CWall *m_pWall;
	static CGame *m_pGame;
	static CRecord *m_pRecord;
	static CItem *m_pItem;
};
#endif