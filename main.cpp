#include <windows.h>
#include "Cheat.h"
#include "main.h"

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Cheat* cheat = new Cheat(L"mspaint.exe");

	vector <int> option1Keys = { VK_NUMPAD1 };
	CheatOption* option1 = new CheatOption(NULL, 
		L"[NUM1] Отключение порога размера всех объектов", 
		option1Keys);
	option1->AddNopPatch(option1Nop_1, 3);
	option1->AddNopPatch(option1Nop_2, 3);
	option1->AddNopPatch(option1Nop_3, 3);
	option1->AddNopPatch(option1Nop_4, 3);
	option1->AddNopPatch(option1Nop_5, 3);
	option1->AddNopPatch(option1Nop_6, 3);
	option1->AddNopPatch(option1Nop_7, 3);
	option1->AddNopPatch(option1Nop_8, 3);
	option1->AddNopPatch(option1Nop_9, 3);
	option1->AddNopPatch(option1Nop_10, 5);

	vector <int> option2Keys = { VK_NUMPAD2 };
	CheatOption* option2 = new CheatOption(NULL, 
		L"[NUM2] Изменение константных размеров  всех объектов",
		option2Keys);
	option2->AddСavePatch(option2Cave_1, option2Bytes_1, 257);
	option2->AddСavePatch(option2Cave_2, option2Bytes_2, 14);

	vector <int> option3Keys = { VK_NUMPAD3 };
	CheatOption* option3 = new CheatOption(NULL, 
		L"[NUM3] Устранение ошибки", 
		option3Keys);
	option3->AddСavePatch(option3Cave_1, option3Bytes_1, 3);

	cheat->AddCheatOption(option1);
	cheat->AddCheatOption(option2);
	cheat->AddCheatOption(option3);

	cheat->Start();
	IRender* renderer = new SimpleRenderer(cheat, 
		windowTitle, 
		W_WIDTH, 
		W_HEIGHT);
	renderer->Start();
	cheat->Stop();

	delete option1;
	delete option2;
	delete option3;
	delete cheat;

	return 0;
}

