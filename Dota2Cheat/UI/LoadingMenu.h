#pragma once
#include <string>
#include <string_view>

namespace Menus {

	inline class LoadingMenu {
		int progress, progressMax;
		std::string task;
	public:
		void SetProgress(int p) {
			if (p <= progressMax)
				progress = p;
		}

		void NewStage(std::string_view title, int progressMax) {
			SetProgress(0);
			task = title;
			this->progressMax = progressMax;
		}

		void Draw();

		void DrawLaunchDialogue();
	} loadMenu;
}