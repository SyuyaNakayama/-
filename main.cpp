#include <math.h>
#include "DxLib.h"
#include "enum.h"
#include "minorFunction.h"
#include "struct.h"
#include "function.h"

// ウィンドウのサイズ
const Screen WIN_SIZE = { 800,800 };

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText("演出王");
	SetGraphMode(WIN_SIZE.x, WIN_SIZE.y, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(20, 20, 20);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)return -1;

	// ---フォントハンドル作成---


	// ---定数の宣言と初期化---
	// 色定数
	const size_t COLOR[] =
	{
		GetColor(255, 255, 255),GetColor(0, 0, 0),
		GetColor(222, 0, 0),GetColor(0, 222, 0),
		GetColor(0, 0, 222)
	};
	const int GROUND_HEIGHT = 700;
	const int PL_SPD = 5;
	const int PARTICLE_NUM = 8;
	const int PARTICLE_SPREAD_SPD = 4;
	const float PARTICLE_ROTATE_SPD = DX_PI_F / 36.0f;

	// ---変数の宣言と初期化---
	// キーボード情報用
	Key keys{};

	Transform player = { {32,32},{WIN_SIZE.x / 2,GROUND_HEIGHT - player.radius.y} };

	Transform particle[PARTICLE_NUM]{};
	bool isParticleExist = 0;
	CAV particleTime{ 60 };
	int particleDis = 0;
	float particleAngle = 0.0f;

	for (size_t i = 0; i < PARTICLE_NUM; i++)
	{
		particle[i].radius.x = 6;
	}

	while (!(ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE)))
	{
		ClearDrawScreen();
		keys.GetKeyState();

		// ---更新処理---
		player.pos.x += (keys.now[KEY_INPUT_RIGHT] - keys.now[KEY_INPUT_LEFT]) * PL_SPD;
		player.pos.y += (keys.now[KEY_INPUT_DOWN] - keys.now[KEY_INPUT_UP]) * PL_SPD;
		Clamp(player.pos.x, player.radius.x, WIN_SIZE.x - player.radius.x);
		Clamp(player.pos.y, player.radius.y, GROUND_HEIGHT - player.radius.y);

		if (keys.PushButtion(KEY_INPUT_SPACE))
		{
			isParticleExist = 1;
		}
		if (isParticleExist)
		{
			particleTime.var--;
			particleDis += PARTICLE_SPREAD_SPD;
			particleAngle += PARTICLE_ROTATE_SPD;

			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle[i].pos.x = player.pos.x + particleDis *
					cosf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particleAngle);
				particle[i].pos.y = player.pos.y + particleDis *
					sinf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particleAngle);
			}

			if (particleTime.var <= 0)
			{
				isParticleExist = 0;
				particleTime.Reset();
				particleDis = 0;
				particleAngle = 0.0f;
			}
		}

		// ---描画処理---
		DrawFillBox(0, GROUND_HEIGHT, WIN_SIZE.x, WIN_SIZE.y, COLOR[BLUE]);
		player.DrawBoxT(COLOR[WHITE]);
		if (isParticleExist)
		{
			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle[i].DrawCircleT(COLOR[RED]);
			}
		}

		ScreenFlip();
	}

	// 全リソースファイル削除
	InitGraph();
	InitSoundMem();

	DxLib_End();
	return 0;
}
