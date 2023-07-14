#include "SceneFactory.h"

#include "GamePlay.h"
#include "Title.h"
#include "Result.h"
#include "SecondStage.h"
#include "StageSelect.h"
#include "ThirdStage.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//Å@éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new Title();
	}

	else if (sceneName == "STAGESELECT")
	{
		newScene = new StageSelect();
	}

	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlay();
	}

	else if (sceneName == "SECONDSTAGE")
	{
		newScene = new SecondStage();
	}

	else if (sceneName == "THIRDSTAGE")
	{
		newScene = new ThirdStage();
	}

	else if (sceneName == "RESULT")
	{
		newScene = new Result();
	}

	return newScene;
}
