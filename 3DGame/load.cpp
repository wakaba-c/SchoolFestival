//========================================================================================
//
// アニメーションロード基本[load.cpp]
// Author : masayasu wakita
//
//========================================================================================
#include "load.h"
#include "player.h"
#include "enemy.h"

//========================================================================================
// マクロ定義
//========================================================================================


//========================================================================================
// グローバル変数
//========================================================================================
LOAD g_aLoad;

int g_nMaxModel = 0;

//========================================================================================
// ポリゴンの初期化
//========================================================================================
void InitLoad(void)
{

}

//========================================================================================
// ポリゴンの初期化
//========================================================================================
int LoadModel(void)
{
	FILE *pFile;
	char cReadText[128];		//文字
	char cHeadText[128];		//比較
	char cDie[128];
	int nCntPointer;			//ポインターの数値
	int nAddPlayer = 0;				//プレイヤーのカウンター
	int nCntkey = 0;				//パーツのカウンター
	int nAnimationType = 0;				//アニメーションの種類

	int nCntMaya = 0;				//mayaのデータ参照

	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//テキストデータロード
	pFile = fopen("data/motion.txt", "r");

	if (pFile != NULL)
	{
		//ポインターのリセット
		nCntPointer = 0;

		//スクリプトが来るまでループ
		while(strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cHeadText, "\n") == 0)
				{

				}

				//モデル数
				if (strcmp(cHeadText, "NUM_MODEL") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDie, &cDie, &g_nMaxModel);
				}

				//モデルのアドレス
				else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
				{
					sscanf(cReadText, "%s %s %s", &cDie, &cDie, &pPlayer->aModel[nAddPlayer].sAdd[0]);
					nAddPlayer++;
				}

				//キャラクターセットだったら
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					pPlayer = GetPlayer();
					nCntPointer = 0;

					//エンドキャラクターセットが来るまでループ
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//パーツセットだったら
						if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							//エンドパーツセットが来るまでループ
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								//インデックス
								if (strcmp(cHeadText, "INDEX") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pPlayer->aModel[nCntPointer].nIndex);
								}

								//親
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pPlayer->aModel[nCntPointer].nParent);
								}

								//位置
								if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, 
										&pPlayer->aModel[nCntPointer].originPos.x,
										&pPlayer->aModel[nCntPointer].originPos.y,
										&pPlayer->aModel[nCntPointer].originPos.z);
								}

								//位置
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pPlayer->aModel[nCntPointer].originRot.x,
										&pPlayer->aModel[nCntPointer].originRot.y,
										&pPlayer->aModel[nCntPointer].originRot.z);
								}
							}

							////モデルの格納場所を進める
							nCntPointer++;
						}
					}
				}

				//キャラクターセットだったら
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					pPlayer = GetPlayer();
					nCntPointer = 0;

					//エンドキャラクターセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//ループするかどうか
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nLoop);

							if (pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nLoop < 0 || pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nLoop > 1)
							{
								//警告
								//MessageBox(hWnd, "ループの値が範囲外です！", "警告！", MB_ICONWARNING);

								//ゲームの終了
								//DestroyWindow(hWnd);
							}
						}

						//キー数
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey);
						}

						//キーセットだったら
						if (strcmp(cHeadText, "KEYSET") == 0)
						{
							//エンドキーセットが来るまでループ
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								//フレーム数
								if (strcmp(cHeadText, "FRAME") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].nFrame);
								}

								//キーだったら
								if (strcmp(cHeadText, "KEY") == 0)
								{
									//エンドキーが来るまでループ
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cReadText, sizeof(cReadText), pFile);
										sscanf(cReadText, "%s", &cHeadText);

										//位置
										if (strcmp(cHeadText, "POS") == 0)
										{
											sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, 
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.x,
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.y,
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.z);
										}

										//回転
										if (strcmp(cHeadText, "ROT") == 0)
										{
											sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.x,
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.y,
												&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.z);
										}
									}

									//モデルの格納場所を進める
									nCntPointer++;
								}
							}

							//keyのカウンターに1プラス
							nCntkey++;
							nCntPointer = 0;

							//プレイヤー情報の更新
							pPlayer = GetPlayer();

							////キーの最大値が超えていないか調べる
							//if (nCntkey > pPlayer->aMotion[MOTIONTYPE_NEUTRAL].nKey)
							//{
							//	//警告
							//	MessageBox(hWnd, "keyの値が範囲外です！", "警告！", MB_ICONWARNING);

							//	//ゲームの終了
							//	DestroyWindow(hWnd);
							//}
						}
					}
					
					//アニメーションの種類に1プラス
					nAnimationType++;


					//keyのカウンターをリセット
					nCntkey = 0;
				}
			}
		}

		//ファイル閉
		fclose(pFile);
	}

	////モデルの数に異常
	//if (nCntPointer != nMaxPlayer)
	//{
	//	//警告
	//	MessageBox(hWnd, "モデルの読み込みに失敗！", "警告！", MB_ICONWARNING);

	//	//ゲームの終了
	//	DestroyWindow(hWnd);
	//}
	//アドレスの数に異常
	if (nAddPlayer != g_nMaxModel + 1)
	{
		//警告
		//MessageBox(hWnd, "アドレスの数が合いません！", "警告！", MB_ICONWARNING);

		//ゲームの終了
		//DestroyWindow(hWnd);
	}

	//開けない
	else
	{
		printf("開けませんでした\n");
	}

	return g_nMaxModel;
}

//========================================================================================
// 敵の初期化
//========================================================================================
int LoadEnemy(void)
{
	FILE *pFile;
	char cReadText[128];		//文字
	char cHeadText[128];		//比較
	char cDie[128];
	int nCntPointer;			//ポインターの数値
	int nAddPlayer = 0;				//プレイヤーのカウンター
	int nCntkey = 0;				//パーツのカウンター
	int nAnimationType = 0;				//アニメーションの種類

	int nCntMaya = 0;				//mayaのデータ参照

	ENEMY *pEnemy;

	pEnemy = GetEnemy();

	//テキストデータロード
	pFile = fopen("data/motion.txt", "r");

	if (pFile != NULL)
	{
		//ポインターのリセット
		nCntPointer = 0;

		//スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cHeadText, "\n") == 0)
				{

				}

				//モデル数
				if (strcmp(cHeadText, "NUM_MODEL") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDie, &cDie, &g_nMaxModel);
				}

				//モデルのアドレス
				else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
				{
					sscanf(cReadText, "%s %s %s", &cDie, &cDie, &pEnemy->aModel[nAddPlayer].sAdd[0]);
					nAddPlayer++;
				}

				//キャラクターセットだったら
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					pEnemy = GetEnemy();
					nCntPointer = 0;

					//エンドキャラクターセットが来るまでループ
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//パーツセットだったら
						if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							//エンドパーツセットが来るまでループ
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								//インデックス
								if (strcmp(cHeadText, "INDEX") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pEnemy->aModel[nCntPointer].nIndex);
								}

								//親
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pEnemy->aModel[nCntPointer].nParent);
								}

								//位置
								if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pEnemy->aModel[nCntPointer].originPos.x,
										&pEnemy->aModel[nCntPointer].originPos.y,
										&pEnemy->aModel[nCntPointer].originPos.z);
								}

								//位置
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pEnemy->aModel[nCntPointer].originRot.x,
										&pEnemy->aModel[nCntPointer].originRot.y,
										&pEnemy->aModel[nCntPointer].originRot.z);
								}
							}

							////モデルの格納場所を進める
							nCntPointer++;
						}
					}
				}

				//キャラクターセットだったら
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					pEnemy = GetEnemy();
					nCntPointer = 0;

					//エンドキャラクターセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//ループするかどうか
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pEnemy->aModel[nCntPointer].aMotion[nAnimationType].nLoop);

							if (pEnemy->aModel[nCntPointer].aMotion[nAnimationType].nLoop < 0 || pEnemy->aModel[nCntPointer].aMotion[nAnimationType].nLoop > 1)
							{
								//警告
								//MessageBox(hWnd, "ループの値が範囲外です！", "警告！", MB_ICONWARNING);

								//ゲームの終了
								//DestroyWindow(hWnd);
							}
						}

						//キー数
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pEnemy->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey);
						}

						//キーセットだったら
						if (strcmp(cHeadText, "KEYSET") == 0)
						{
							//エンドキーセットが来るまでループ
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								//フレーム数
								if (strcmp(cHeadText, "FRAME") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDie, &cDie, &pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].nFrame);
								}

								//キーだったら
								if (strcmp(cHeadText, "KEY") == 0)
								{
									//エンドキーが来るまでループ
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cReadText, sizeof(cReadText), pFile);
										sscanf(cReadText, "%s", &cHeadText);

										//位置
										if (strcmp(cHeadText, "POS") == 0)
										{
											sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.x,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.y,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].pos.z);
										}

										//回転
										if (strcmp(cHeadText, "ROT") == 0)
										{
											sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.x,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.y,
												&pEnemy->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntkey].rot.z);
										}
									}

									//モデルの格納場所を進める
									nCntPointer++;
								}
							}

							//keyのカウンターに1プラス
							nCntkey++;
							nCntPointer = 0;

							//プレイヤー情報の更新
							pEnemy = GetEnemy();

							////キーの最大値が超えていないか調べる
							//if (nCntkey > pPlayer->aMotion[MOTIONTYPE_NEUTRAL].nKey)
							//{
							//	//警告
							//	MessageBox(hWnd, "keyの値が範囲外です！", "警告！", MB_ICONWARNING);

							//	//ゲームの終了
							//	DestroyWindow(hWnd);
							//}
						}
					}

					//アニメーションの種類に1プラス
					nAnimationType++;


					//keyのカウンターをリセット
					nCntkey = 0;
				}
			}
		}

		//ファイル閉
		fclose(pFile);
	}

	////モデルの数に異常
	//if (nCntPointer != nMaxPlayer)
	//{
	//	//警告
	//	MessageBox(hWnd, "モデルの読み込みに失敗！", "警告！", MB_ICONWARNING);

	//	//ゲームの終了
	//	DestroyWindow(hWnd);
	//}
	//アドレスの数に異常
	if (nAddPlayer != g_nMaxModel + 1)
	{
		//警告
		//MessageBox(hWnd, "アドレスの数が合いません！", "警告！", MB_ICONWARNING);

		//ゲームの終了
		//DestroyWindow(hWnd);
	}

	//開けない
	else
	{
		printf("開けませんでした\n");
	}

	return g_nMaxModel;
}

//========================================================================================
// mayaからのデータロード
//========================================================================================
void LoadMaya(void)
{
	FILE *pFile;
	char cReadText[128];		//文字
	char cReadType[128];		//タイプ
	char cHeadText[128];		//比較
	char cBodyType[128];		//parts
	char cDie[128];				//削除
	int nCntPointer;			//ポインターの数値
	int nCntKey = 0;			//キー数//パーツのカウンター
	int nMaxPlayer = NULL;
	int nAddPlayer = 0;			//プレイヤーのカウンター
	int nAnimationType = MOTIONTYPE_NEUTRAL;		//アニメーションの種類

	int nCntMaya = 0;			//mayaのデータ参照

	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//=============== Maya ==================//
	//テキストデータロード
	pFile = fopen("data/test.anim", "r");
	nCntPointer = 0;

	//開けた時
	if (pFile != NULL)
	{
		while (14 > nCntPointer)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s %s %s %s", &cHeadText, &cReadType, &cDie, &cBodyType);

			//================== 移動Xの情報だった時 ==================//
			if (strcmp(cBodyType, "body") == 0 && strcmp(cReadType, "translate.translateX") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

				//移動Xの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%s %f",
						&cDie,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].pos.x);

					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
			}

			//================== 移動Yの情報だった時 ==================//
			else if (strcmp(cBodyType, "body") == 0 && strcmp(cReadType, "translate.translateY") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

				//移動Yの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%s %f",
						&cDie,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].pos.y);

					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
			}

			//================== 移動Zの情報だった時 ==================//
			else if (strcmp(cBodyType, "body") == 0 && strcmp(cReadType, "translate.translateZ") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

				//移動Zの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%s %f",
						&cDie,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].pos.z);

					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
			}

			//================== 回転Xの情報だった時 ==================//
			else if (strcmp(cReadType, "rotate.rotateX") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

				//回転Xの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%d %f",
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].nFrame,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].rot.x);
					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].nFrame -= pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey - 1].nFrame;

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
			}

			//================== 回転Xの情報だった時 ==================//
			else if (strcmp(cReadType, "rotate.rotateY") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

				//移動Yの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%s %f",
						&cDie,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].rot.y);

					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
			}

			//================== 回転Zの情報だった時 ==================//
			else if (strcmp(cReadType, "rotate.rotateZ") == 0)
			{
				//キーまで飛ばす
				while ((strcmp(cHeadText, "keys") != 0))
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);
				}

				//改行
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

				nCntKey = 0;				//キー数の初期化

											//移動Xの読み込み
				while ((strcmp(cHeadText, "}") != 0))
				{
					sscanf(cReadText, "%s %f",
						&cDie,
						&pPlayer->aModel[nCntPointer].aMotion[nAnimationType].aKey[nCntKey].rot.z);

					//改行
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s %s", &cHeadText, &cReadType);

					nCntKey++;				//キー数の加算
				}

				//キー数
				pPlayer->aModel[nCntPointer].aMotion[nAnimationType].nMaxKey = nCntKey;
				nCntPointer++;
			}
		}
	}
	else
	{
		printf("開けませんでした\n");
	}
}